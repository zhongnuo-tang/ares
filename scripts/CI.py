#!/usr/bin/env python3
# This script should be executed from the 'ares/scripts' directory.

import os
import subprocess
import tarfile
import re
import requests
import argparse
import sys
import glob

ARES_REPO = "https://github.com/EangJS/ares.git"
TIZENRT_REPO = "https://github.com/Samsung/TizenRT.git"
BUILD_DIR = "/tmp/build"
TIZENRT_DIR = BUILD_DIR + "/TizenRT"
ARES_DIR = TIZENRT_DIR + "/" + "apps/examples/ares"
PATCH_DIR = ARES_DIR + "/scripts/patches"

BOARD = "rtl8730e"
CONFIGS = ["ares_ddr", "ares_ddr_st7785"]
ARTIFACTS = ["km4_boot_all.bin", "*.trpk", "bootparam.bin", "target_img2.axf"]

def run(cmd, cwd=None):
    print(f"\n>>> {cmd}")
    result = subprocess.run(cmd, shell=True, check=True, cwd=cwd)
    print(f">>> Command finished with return code {result.returncode}")
    
def zip_directory(src_dir, tar_path):
    print(f"Compressing {src_dir} -> {tar_path}")
    with tarfile.open(tar_path, "w:xz") as tar:
        tar.add(src_dir, arcname=os.path.basename(src_dir))
                
def get_next_build_number(repo_url, repo_name, auth):
    """
    Get next build number by listing existing assets in the raw repo.
    """
    builds = set()
    continuation_token = None

    while True:
        params = {"repository": repo_name}
        if continuation_token:
            params["continuationToken"] = continuation_token

        url = f"{repo_url}/service/rest/v1/assets"
        resp = requests.get(url, params=params, auth=auth)
        resp.raise_for_status()
        data = resp.json()

        for item in data.get("items", []):
            path = item.get("path", "")
            m = re.match(rf"{repo_name}/build_(\d+)/", path)
            if m:
                builds.add(int(m.group(1)))

        continuation_token = data.get("continuationToken")
        if not continuation_token:
            break

    return max(builds) + 1 if builds else 1


def upload_to_nexus_raw(repo_url, repo_name, file_path, directory, auth=None):
    """
    Upload a file to Nexus Raw repo with authentication
    """
    filename = os.path.basename(file_path)
    url = f"{repo_url}/service/rest/v1/components?repository={repo_name}"

    files = {
        "raw.asset1": (filename, open(file_path, "rb"), "application/zip")
    }

    data = {
        "raw.directory": directory,
        "raw.asset1.filename": filename  # THIS IS REQUIRED
    }

    response = requests.post(url, data=data, files=files, auth=auth)

    if response.status_code not in (200, 201, 204):
        raise RuntimeError(f"Upload failed: {response.status_code} {response.text}")

    print(f"Uploaded {file_path} to {directory} successfully")
    

def clone(os_dir):
    # Clone repo if not already present
    if not os.path.isdir(TIZENRT_DIR):
        run(f"git clone {TIZENRT_REPO} {TIZENRT_DIR}")
    else:
        print("TizenRT already cloned, skipping clone.")
    
    run(f"mkdir -p {ARES_DIR}")
    run(f"cp -r . {ARES_DIR}/", cwd="..")
    
    run("git submodule update --init --recursive", cwd=ARES_DIR)

    if not os.path.isdir(os_dir):
        print("ERROR: os directory not found. Repo layout unexpected.")
        sys.exit(1)
        
def verify_build():
    bin_dir = os.path.join(TIZENRT_DIR, "build", "output", "bin")
    for artifact in ARTIFACTS:
        pattern = os.path.join(bin_dir, artifact)
        matches = glob.glob(pattern)

        if not matches:
            print(f"ERROR: Build output missing essential artifact: {artifact}")
            sys.exit(1)

def cleanup_artifacts():
    bin_dir = os.path.join(TIZENRT_DIR, "build", "output", "bin")

    allowed_files = set()
    for artifact in ARTIFACTS:
        pattern = os.path.join(bin_dir, artifact)
        allowed_files.update(
            os.path.basename(p) for p in glob.glob(pattern)
        )

    for filename in os.listdir(bin_dir):
        file_path = os.path.join(bin_dir, filename)

        if os.path.isfile(file_path) and filename not in allowed_files:
            os.remove(file_path)

def apply_patches():
    patch_files = [
        f for f in os.listdir(PATCH_DIR) if f.endswith('.patch')
    ]
    for patch_file in patch_files:
        patch_path = os.path.join(PATCH_DIR, patch_file)
        if "tizen" in patch_file.lower():
            run(f"git apply {patch_path}", cwd=TIZENRT_DIR)
        elif "ares" in patch_file.lower():
            run(f"git apply {patch_path}", cwd=ARES_DIR)

# =========================
# Main logic
# =========================
def main(args):
    os_dir = os.path.join(TIZENRT_DIR, "os")
    clone(os_dir)
    apply_patches()
    run("chmod +x tools/configure.sh", cwd=os_dir)

    for config in CONFIGS:
        BOARD_CONFIG = f"{BOARD}/{config}"
        print(f"\n=== Building configuration: {BOARD_CONFIG} ===")

        run(f"cp -r {config} {TIZENRT_DIR}/build/configs/{BOARD}/")
        try:
            run(f"./dbuild.sh distclean", cwd=os_dir)
            run(f"./tools/configure.sh {BOARD_CONFIG}", cwd=os_dir)
        except subprocess.CalledProcessError:
            pass

        run("./dbuild.sh", cwd=os_dir)
        verify_build()
        cleanup_artifacts()
        print("\nBuild complete!")
        run(f"ls -l build/output/bin", cwd=TIZENRT_DIR)
        run(f"mkdir -p {BUILD_DIR}/assets")
        zip_path = f"{BUILD_DIR}/assets/ares_{config}.tar.xz"
        zip_directory(f"{TIZENRT_DIR}/build/output/bin", zip_path)
        
    if args.upload_url:
        assets_zip_path = f"{BUILD_DIR}/ares.tar.xz"
        zip_directory(f"{BUILD_DIR}/assets", assets_zip_path)
        auth=(
                args.username,
                args.password
            ) if args.username and args.password else None
        build_number = get_next_build_number(args.upload_url, "Ares", auth)
        upload_to_nexus_raw(
            repo_url=args.upload_url,
            repo_name="Ares",
            file_path=assets_zip_path,
            directory=f"build_{build_number}",
            auth=auth,
        )

    run(f"sudo rm -rf {BUILD_DIR}/TizenRT")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Clone, build, zip, and upload TizenRT")
    parser.add_argument(
        "--upload-url",
        required=False,
        help="Destination URL to upload the zip file (HTTP POST)",
    )
    parser.add_argument(
        "--username",
        required=False,
        help="Username for authentication (if required)",
    )
    parser.add_argument(
        "--password",
        required=False,
        help="Password for authentication (if required)",
    )

    args = parser.parse_args()
    main(args)
