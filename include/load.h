#ifndef LOAD_H
#define LOAD_H

int map_trace(const char *name, void **start, void **end, size_t *size);
struct heap* load(char **files, int no_files, unsigned int *count);

struct evlink {
	struct evlink *next;
	struct st_event_record* rec;
};

struct task {
	int pid;
	unsigned int no_events;
	struct st_event_record* name;
	struct st_event_record* param;
	struct evlink* events;
	struct evlink** next;
};

#define MAX_TASKS 100

extern struct task tasks[MAX_TASKS];
extern u64 time0;
extern u32 g_min_task;
extern u32 g_max_task;

static inline double ns2ms(u64 ns)
{
	return ns / 1000000.0;
}

static inline double ns2ms_adj(u64 ns)
{
	return ns2ms(ns - time0);
}

static inline double evtime(struct st_event_record* rec)
{
	return ns2ms_adj(event_time(rec));
}


void crop_events(struct task* t, double min, double max);
void crop_events_all(double min, double max);

struct task* by_pid(int pid);

void split(struct heap* h, unsigned int count);

const char* tsk_name(struct task* t);
u32 per(struct task* t);
u32 exe(struct task* t);
u32 idx(struct task* t);

u32 count_tasks(void);

#define CHECK(_e, test) if (test) fprintf(stderr, "'%s' failed.\n", #test);

#define for_each_task(t) \
	for (t = tasks + g_min_task; t < tasks + g_max_task && t->pid; t++)

#define for_each_event(t, e) \
	for (e = t->events; e; e = e->next)

#define for_each_event_t(t, e, evtype) \
	for_each_event(t, e) if (e->rec->hdr.type == evtype)

#define find(e, evtype) \
	while (e && e->rec->hdr.type != evtype) e = e->next; if (!e) break;


#endif
