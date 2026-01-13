/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include "task_manager.h"


/****************************************************************************
 * ares_main
 ****************************************************************************/


#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int ares_main(int argc, char *argv[])
#endif
{
	printf("ares start4!!\n");
	run_tasks();
	return 0;
}
