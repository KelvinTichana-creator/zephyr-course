#include <zephyr/init.h>
#include <zephyr/sys/printk.h>

static int board_initialized(void)
{
    printk("Board Initialized\n");
    return 0;
}

SYS_INIT(board_initialized, POST_KERNEL, 0);
