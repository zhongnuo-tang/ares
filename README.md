# Ares

TizenRT application which includes various functional tests.

## Prerequisites
1. AILITE BOARD
2. Docker
3. GIT

## Usage

1. Clone the [TizenRT](https://github.com/Samsung/TizenRT) repository
2. `cd` into `apps/examples` of the cloned TizenRT repository
3. Clone this repository into the CWD

## Building the application

1. `cd` into `os/` directory of the TizenRT repository
2. Run `./dbuild.sh menu`
3. Ensure configuration is set as:
  1. RTL8730E
  2. loadable_ext_xxx (Depending on board type)
4. Run `"3. Modify Current Configuration"`
5. In `Application Configuration > Examples`, enable `Ares` example.
6. Run `1. Build with Current Configuration`
