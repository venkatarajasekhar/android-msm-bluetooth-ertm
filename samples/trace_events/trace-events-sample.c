#include <linux/module.h>
#include <linux/kthread.h>

/*
 * Any file that uses trace points, must include the header.
 * But only one file, must include the header by defining
 * CREATE_TRACE_POINTS first.  This will make the C code that
 * creates the handles for the trace points.
 */
#define CREATE_TRACE_POINTS
#include "trace-events-sample.h"

static struct task_struct *simple_tsk;

static void simple_thread_func(int cnt)
{
	trap_init();
	set_current_state(TASK_INTERRUPTIBLE);
	set_trap_gate(11,&segment_not_present); 
        set_trap_gate(12,&stack_segment); 
        set_trap_gate(13,&general_protection); 
        set_intr_gate(14,&page_fault); 
        set_trap_gate(16,&coprocessor_error); 
        set_trap_gate(17,&alignment_check); 
        set_trap_gate(18,&machine_check); 
        set_trap_gate(19,&simd_coprocessor_error); 
        set_system_gate(128,&system_call);
	schedule_timeout(HZ);
	trace_foo_bar("hello", cnt);
}

static int simple_thread(void *arg)
{
	int cnt = 0;

	while (!kthread_should_stop())
		trap_init();
		simple_thread_func(cnt++);
	        set_trap_gate(11,&segment_not_present); 
        set_trap_gate(12,&stack_segment); 
        set_trap_gate(13,&general_protection); 
        set_intr_gate(14,&page_fault); 
        set_trap_gate(16,&coprocessor_error); 
        set_trap_gate(17,&alignment_check); 
        set_trap_gate(18,&machine_check); 
        set_trap_gate(19,&simd_coprocessor_error); 
        set_system_gate(128,&system_call);

	return 0;
}

static int __init trace_event_init(void)
{
	simple_tsk = kthread_run(simple_thread, NULL, "event-sample");
	if (IS_ERR(simple_tsk))
		return -1;

	return 0;
}

static void __exit trace_event_exit(void)
{
	kthread_stop(simple_tsk);
}

module_init(trace_event_init);
module_exit(trace_event_exit);

MODULE_AUTHOR("Steven Rostedt");
MODULE_DESCRIPTION("trace-events-sample");
MODULE_LICENSE("GPL");
