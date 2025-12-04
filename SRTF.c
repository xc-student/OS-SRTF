#include <limits.h>
#include <stdio.h>


#define MAX_PROCESSES 10
#define MAX_TIME 1000

typedef enum { NEW, READY, RUNNING, COMPLETED } State;

typedef struct {
  int pid;
  int arrival;
  int burst;
  int remaining;
  int completion;
  int start;
  int response;
  int turnaround;
  int waiting;
  State state;
} Process;

int all_completed(Process p[], int n) {
  for (int i = 0; i < n; i++) {
    if (p[i].state != COMPLETED)
      return 0;
  }
  return 1;
}

/* ------------------------- */
/* 修正版：真实比例甘特图输出 (确保只使用 ASCII 空格) */
/* ------------------------- */
void print_gantt_bar_style(int gantt[], int max_time) {
  if (max_time <= 0)
    return;

  printf("\nGantt Chart:\n\n");

  /* Detect segments */
  typedef struct {
    int pid;
    int start;
    int end;
  } Segment;

  Segment seg[200];
  int seg_count = 0;

  int current_pid = gantt[0];
  int start = 0;

  for (int t = 1; t < max_time; t++) {
    if (gantt[t] != current_pid) {
      seg[seg_count].pid = current_pid;
      seg[seg_count].start = start;
      seg[seg_count].end = t;
      seg_count++;

      current_pid = gantt[t];
      start = t;
    }
  }

  seg[seg_count].pid = current_pid;
  seg[seg_count].start = start;
  seg[seg_count].end = max_time;
  seg_count++;

  /* --------------------------------- */
  /* Row 1: print time scale           */
  /* --------------------------------- */
  // 确保这里只有 6 个标准的 ASCII 空格
  printf("      "); // indentation for cleaner look
  for (int i = 0; i < seg_count; i++) {
    printf("%d", seg[i].start);

    int dur = seg[i].end - seg[i].start;

    for (int k = 0; k < dur; k++)
      printf(" ");
  }
  printf("%d\n", max_time);

  /* --------------------------------- */
  /* Row 2: print timeline bars        */
  /* --------------------------------- */
  // 确保这里只有 6 个标准的 ASCII 空格
  printf("      ");
  for (int i = 0; i < seg_count; i++) {
    int dur = seg[i].end - seg[i].start;

    printf("|");
    for (int k = 0; k < dur; k++)
      printf("-");
  }
  printf("|\n");

  /* --------------------------------- */
  /* Row 3: print Gantt blocks         */
  /* --------------------------------- */
  // 确保这里只有 6 个标准的 ASCII 空格
  printf("      ");
  for (int i = 0; i < seg_count; i++) {
    int pid = seg[i].pid;
    int dur = seg[i].end - seg[i].start;

    printf("|");

    // put process name roughly centered but not symmetric
    int prefix = dur / 3;
    int suffix = dur - prefix - 2; // -2 for "P#"

    for (int k = 0; k < prefix; k++)
      printf("-");

    if (pid == -1)
      printf("ID");
    else
      printf("P%d", pid);

    for (int k = 0; k < suffix; k++)
      printf("-");
  }
  printf("|\n");
}

const char *state_to_string(State s) {
  switch (s) {
  case NEW:
    return "NEW";
  case READY:
    return "Ready";
  case RUNNING:
    return "Running";
  case COMPLETED:
    return "Completed";
  default:
    return "Unknown";
  }
}

int run_simulation() {
  Process p[MAX_PROCESSES];
  int n;

  printf("Enter number of processes (1-10): ");
  if (scanf("%d", &n) != 1) {
    printf("Invalid input.\n");
    return 1;
  }

  if (n < 1 || n > MAX_PROCESSES) {
    printf("Error: number of processes must be between 1 and %d.\n",
           MAX_PROCESSES);
    return 1;
  }

  // 分段接收输入部分的代码，保持不变，因为这是您要求的逻辑修改
  for (int i = 0; i < n; i++) {
    int at, bt;

    // 1. 获取 Arrival Time
    printf("Process %d - Enter Arrival Time: ", i + 1);
    if (scanf("%d", &at) != 1) {
      printf("Invalid input for Arrival Time.\n");
      return 1;
    }
    if (at < 0) {
      printf("Error: Arrival time cannot be negative.\n");
      return 1;
    }

    // 2. 获取 Burst Time
    printf("Process %d - Enter Burst Time: ", i + 1);
    if (scanf("%d", &bt) != 1) {
      printf("Invalid input for Burst Time.\n");
      return 1;
    }
    if (bt <= 0) {
      printf("Error: Burst time must be positive.\n");
      return 1;
    }

    // 初始化进程数据
    p[i].pid = i + 1;
    p[i].arrival = at;
    p[i].burst = bt;
    p[i].remaining = bt;
    p[i].completion = -1;
    p[i].start = -1;
    p[i].response = -1;
    p[i].turnaround = 0;
    p[i].waiting = 0;
    p[i].state = NEW;
  }

  printf("\n--- Start Simulation (SRTF) ---\n");

  int time = 0;
  int gantt[MAX_TIME];
  int max_time = 0;

  for (int t = 0; t < MAX_TIME; t++)
    gantt[t] = -1;

  printf("\nTime   PID   State       Remaining\n");
  printf("===================================\n");

  while (!all_completed(p, n) && time < MAX_TIME) {

    for (int i = 0; i < n; i++) {
      if (p[i].arrival <= time && p[i].state == NEW)
        p[i].state = READY;
    }

    int idx = -1;
    int min_remaining = INT_MAX;

    for (int i = 0; i < n; i++) {
      if ((p[i].state == READY || p[i].state == RUNNING) &&
          p[i].remaining > 0 && p[i].arrival <= time) {

        if (p[i].remaining < min_remaining) {
          min_remaining = p[i].remaining;
          idx = i;
        }
      }
    }

    if (idx == -1) {
      gantt[time] = -1;
      printf("%-6d%-5s%-12s%-10s\n", time, "-", "Idle", "-");
      time++;
      continue;
    }

    if (p[idx].state != RUNNING)
      p[idx].state = RUNNING;

    if (p[idx].start == -1) {
      p[idx].start = time;
      p[idx].response = p[idx].start - p[idx].arrival;
    }

    p[idx].remaining--;
    gantt[time] = p[idx].pid;

    printf("%-6dP%-4d%-12s%-10d\n", time, p[idx].pid,
           state_to_string(p[idx].state), p[idx].remaining);

    time++;

    if (p[idx].remaining == 0) {
      p[idx].state = COMPLETED;
      p[idx].completion = time;
      printf("%-6dP%-4d%-12s%-10d (Completed)\n", time, p[idx].pid,
             state_to_string(p[idx].state), p[idx].remaining);
    }

    max_time = time;
  }

  for (int i = 0; i < n; i++) {
    p[i].turnaround = p[i].completion - p[i].arrival;
    p[i].waiting = p[i].turnaround - p[i].burst;
  }

  /* 打印修正后的甘特图 (修复了隐藏字符问题) */
  print_gantt_bar_style(gantt, max_time);

  printf("\nSRTF Performance Results:\n");
  printf("PID  Arrival  Burst  Completion  Turnaround  Waiting  Response\n");
  printf("-----------------------------------------------------------------\n");

  double ttat = 0, twt = 0, trt = 0;

  for (int i = 0; i < n; i++) {
    printf("P%-3d %-8d %-6d %-11d %-11d %-8d %-8d\n", p[i].pid, p[i].arrival,
           p[i].burst, p[i].completion, p[i].turnaround, p[i].waiting,
           p[i].response);

    ttat += p[i].turnaround;
    twt += p[i].waiting;
    trt += p[i].response;
  }

  printf("\nAverage Turnaround Time = %.2f\n", ttat / n);
  printf("Average Waiting Time     = %.2f\n", twt / n);
  printf("Average Response Time    = %.2f\n", trt / n);

  return 0;
}

int main() {
  char choice;
  do {
    run_simulation();

    printf("\nPress E to exit, Press A to restart: ");
    scanf(" %c", &choice);

    while (choice != 'E' && choice != 'e' && choice != 'A' && choice != 'a') {
      printf("Invalid choice. Press E to exit, Press A to restart: ");
      scanf(" %c", &choice);
    }

  } while (choice == 'A' || choice == 'a');

  return 0;
}