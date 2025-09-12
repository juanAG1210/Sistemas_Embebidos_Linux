#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#define MAX_LINE 256
#define MAX_CORES 64

typedef struct {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
} CpuStats;

CpuStats prev_stats[MAX_CORES];
int num_cores = 0;

// Leer CPU info desde /proc/stat
int read_cpu_stats(CpuStats stats[]) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

    char line[MAX_LINE];
    int core = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "cpu", 3) == 0 && line[3] != ' ') {
            sscanf(line, "cpu%d %llu %llu %llu %llu %llu %llu %llu %llu",
                   &core,
                   &stats[core].user, &stats[core].nice, &stats[core].system,
                   &stats[core].idle, &stats[core].iowait,
                   &stats[core].irq, &stats[core].softirq, &stats[core].steal);
            core++;
        }
    }

    fclose(fp);
    return core;
}

// Calcular uso de CPU
double calc_cpu_usage(CpuStats prev, CpuStats curr) {
    unsigned long long prev_idle = prev.idle + prev.iowait;
    unsigned long long idle = curr.idle + curr.iowait;

    unsigned long long prev_total = prev.user + prev.nice + prev.system +
                                   prev_idle + prev.irq + prev.softirq + prev.steal;
    unsigned long long total = curr.user + curr.nice + curr.system +
                              idle + curr.irq + curr.softirq + curr.steal;

    unsigned long long totald = total - prev_total;
    unsigned long long idled = idle - prev_idle;

    return (totald - idled) * 100.0 / totald;
}

// Obtener memoria
void get_memory_info(long *mem_total, long *mem_free, long *swap_total, long *swap_free) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return;

    char key[MAX_LINE];
    long value;
    char unit[8];

    while (fscanf(fp, "%s %ld %s\n", key, &value, unit) == 3) {
        if (strcmp(key, "MemTotal:") == 0) *mem_total = value;
        else if (strcmp(key, "MemAvailable:") == 0) *mem_free = value;
        else if (strcmp(key, "SwapTotal:") == 0) *swap_total = value;
        else if (strcmp(key, "SwapFree:") == 0) *swap_free = value;
    }
    fclose(fp);
}

// Obtener CPU info
void get_cpu_info(char *model, int *cores) {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) return;

    char line[MAX_LINE];
    *cores = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0 && strlen(model) == 0) {
            char *p = strchr(line, ':');
            if (p) strncpy(model, p + 2, MAX_LINE - 1);
        }
        if (strncmp(line, "processor", 9) == 0) {
            (*cores)++;
        }
    }
    fclose(fp);
}

// Main
int main() {
    initscr();
    noecho();
    curs_set(FALSE);

    char model[MAX_LINE] = {0};
    get_cpu_info(model, &num_cores);

    CpuStats curr_stats[MAX_CORES];
    read_cpu_stats(prev_stats); // Inicial

    while (1) {
        clear();

        // Info memoria
        long mem_total = 0, mem_free = 0, swap_total = 0, swap_free = 0;
        get_memory_info(&mem_total, &mem_free, &swap_total, &swap_free);

        mvprintw(0, 0, "CPU: %s", model);
        mvprintw(1, 0, "Cores: %d", num_cores);
        mvprintw(2, 0, "Mem Total: %ld MB", mem_total / 1024);
        mvprintw(3, 0, "Mem Used:  %ld MB", (mem_total - mem_free) / 1024);
        mvprintw(4, 0, "Swap Total: %ld MB", swap_total / 1024);
        mvprintw(5, 0, "Swap Used:  %ld MB", (swap_total - swap_free) / 1024);

        // CPU usage
        read_cpu_stats(curr_stats);
        for (int i = 0; i < num_cores; i++) {
            double usage = calc_cpu_usage(prev_stats[i], curr_stats[i]);
            mvprintw(7 + i, 0, "CPU%d: %.2f%%", i, usage);
        }
        memcpy(prev_stats, curr_stats, sizeof(prev_stats));

        refresh();
        sleep(2);
    }

    endwin();
    return 0;
}
