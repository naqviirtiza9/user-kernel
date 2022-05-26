#include<linux/kernel.h>
#include<linux/syscalls.h>
#include<linux/linkage.h>
asmlinkage long sys_prodcon(int type, int num, int productloc, int item)
{
	if(type==1)
	{
		printk("the consumer %d has consumed an item %d from loc %d\n",num,item,productloc);
	}
	if(type==2)
	{
		printk("the producer %d has produced an item %d at loc %d\n",num,item,productloc);
	}
	return 0;
}
