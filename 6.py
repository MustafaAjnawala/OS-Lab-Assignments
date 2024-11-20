# import os
# import random

import matplotlib.pyplot as plt
import numpy as np


class Process:
    def __init__(self, pid, arrival_time, burst_time, priority):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.priority = priority
        self.remaining_time = burst_time
        self.completion_time = 0
        self.turnaround_time = 0
        self.waiting_time = 0
        
def fcfs(processes):
    sorted_processes = sorted(processes, key=lambda p: p.arrival_time)
    time = 0
    gantt_chart = []
    for process in sorted_processes:
        if time < process.arrival_time:
            gantt_chart.append(("Idle", time, process.arrival_time))
            time = process.arrival_time
        gantt_chart.append((process.pid, time, time + process.burst_time))
        time += process.burst_time
        process.completion_time = time
        process.turnaround_time = process.completion_time - process.arrival_time
        process.waiting_time = process.turnaround_time - process.burst_time
    return gantt_chart


def round_robin(processes, quantum):
    sorted_processes = sorted(processes, key=lambda p: p.arrival_time)
    time = 0
    queue = []
    gantt_chart = []
    remaining_processes = [Process(p.pid, p.arrival_time, p.burst_time, p.priority) for p in sorted_processes]

    while remaining_processes or queue:
        # Add newly arrived processes to the queue
        while remaining_processes and remaining_processes[0].arrival_time <= time:
            queue.append(remaining_processes.pop(0))

        if not queue:
            # If no process in queue, move time to next process arrival
            if remaining_processes:
                next_arrival = remaining_processes[0].arrival_time
                gantt_chart.append(("Idle", time, next_arrival))
                time = next_arrival
                continue
            else:
                break

        process = queue.pop(0)
        if process.remaining_time > quantum:
            gantt_chart.append((process.pid, time, time + quantum))
            time += quantum
            process.remaining_time -= quantum
            # Add newly arrived processes before re-adding the current process
            while remaining_processes and remaining_processes[0].arrival_time <= time:
                queue.append(remaining_processes.pop(0))
            queue.append(process)
        else:
            gantt_chart.append((process.pid, time, time + process.remaining_time))
            time += process.remaining_time
            process.remaining_time = 0
            process.completion_time = time
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time

    return gantt_chart


def non_preemptive_sjf(processes):
    sorted_processes = sorted(processes, key=lambda p: p.arrival_time)
    time = 0
    gantt_chart = []
    completed = []
    remaining_processes = [Process(p.pid, p.arrival_time, p.burst_time, p.priority) for p in sorted_processes]

    while remaining_processes or len(completed) < len(processes):
        available_processes = [p for p in remaining_processes if p.arrival_time <= time]
        if available_processes:
            process = min(available_processes, key=lambda p: p.burst_time)
            gantt_chart.append((process.pid, time, time + process.burst_time))
            time += process.burst_time
            process.completion_time = time
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time
            completed.append(process)
            remaining_processes.remove(process)
        else:
            next_arrival = min(p.arrival_time for p in remaining_processes)
            gantt_chart.append(("Idle", time, next_arrival))
            time = next_arrival

    return gantt_chart


def preemptive_sjf(processes):
    sorted_processes = sorted(processes, key=lambda p: p.arrival_time)
    time = 0
    gantt_chart = []
    remaining_processes = [Process(p.pid, p.arrival_time, p.burst_time, p.priority) for p in sorted_processes]
    current_process = None
    last_switch_time = 0
    completed_processes = []

    while remaining_processes or current_process:
        # Get available processes
        available_processes = [p for p in remaining_processes if p.arrival_time <= time]

        if not available_processes and not current_process:
            # If no process is available or running, move time to next process arrival
            next_arrival = min(p.arrival_time for p in remaining_processes)
            gantt_chart.append(("Idle", time, next_arrival))
            time = next_arrival
            continue

        # Find the process with the shortest remaining time among available and current processes
        if current_process:
            available_processes.append(current_process)

        shortest_job = min(available_processes, key=lambda p: p.remaining_time)

        if current_process != shortest_job:#If shortest_job is different from the current_process, a switch occurs
            if current_process:
                gantt_chart.append((current_process.pid, last_switch_time, time))
                if current_process.remaining_time > 0:
                    remaining_processes.append(current_process)
            current_process = shortest_job
            last_switch_time = time
            if current_process in remaining_processes:
                remaining_processes.remove(current_process)

        current_process.remaining_time -= 1 #The current_process executes for one time unit 
        time += 1

        if current_process.remaining_time == 0:
            gantt_chart.append((current_process.pid, last_switch_time, time))
            current_process.completion_time = time
            current_process.turnaround_time = current_process.completion_time - current_process.arrival_time
            current_process.waiting_time = current_process.turnaround_time - current_process.burst_time
            completed_processes.append(current_process)
            current_process = None
            last_switch_time = time

    # Update the original process objects with the calculated times
    for completed_process in completed_processes:
        for original_process in processes:
            if original_process.pid == completed_process.pid:
                original_process.completion_time = completed_process.completion_time
                original_process.turnaround_time = completed_process.turnaround_time
                original_process.waiting_time = completed_process.waiting_time
                break

    return gantt_chart


def non_preemptive_priority(processes):
    sorted_processes = sorted(processes, key=lambda p: p.arrival_time)
    time = 0
    gantt_chart = []
    completed = []
    remaining_processes = [Process(p.pid, p.arrival_time, p.burst_time, p.priority) for p in sorted_processes]

    while remaining_processes or len(completed) < len(processes):
        available_processes = [p for p in remaining_processes if p.arrival_time <= time]
        if available_processes:
            process = min(available_processes, key=lambda p: p.priority)
            gantt_chart.append((process.pid, time, time + process.burst_time))
            time += process.burst_time
            process.completion_time = time
            process.turnaround_time = process.completion_time - process.arrival_time
            process.waiting_time = process.turnaround_time - process.burst_time
            completed.append(process)
            remaining_processes.remove(process)
        else:
            next_arrival = min(p.arrival_time for p in remaining_processes)
            gantt_chart.append(("Idle", time, next_arrival))
            time = next_arrival

    return gantt_chart





def preemptive_priority(processes):
    sorted_processes = sorted(processes, key=lambda p: p.arrival_time)
    time = 0
    gantt_chart = []
    remaining_processes = [Process(p.pid, p.arrival_time, p.burst_time, p.priority) for p in sorted_processes]
    current_process = None
    last_switch_time = 0
    completed_processes = []

    while remaining_processes or current_process:
        available_processes = [p for p in remaining_processes if p.arrival_time <= time]

        if not available_processes and not current_process:
            next_arrival = min(p.arrival_time for p in remaining_processes)
            gantt_chart.append(("Idle", time, next_arrival))
            time = next_arrival
            continue

        if available_processes:
            highest_priority_process = min(available_processes, key=lambda p: p.priority)
            if not current_process or highest_priority_process.priority < current_process.priority:
                if current_process:
                    gantt_chart.append((current_process.pid, last_switch_time, time))
                    remaining_processes.append(current_process)
                current_process = highest_priority_process
                last_switch_time = time
                remaining_processes.remove(highest_priority_process)

        if current_process:
            current_process.remaining_time -= 1
            time += 1

            if current_process.remaining_time == 0:
                gantt_chart.append((current_process.pid, last_switch_time, time))
                current_process.completion_time = time
                current_process.turnaround_time = current_process.completion_time - current_process.arrival_time
                current_process.waiting_time = current_process.turnaround_time - current_process.burst_time
                completed_processes.append(current_process)
                current_process = None
                last_switch_time = time

    # Update the original process objects with the calculated times
    for completed_process in completed_processes:
        for original_process in processes:
            if original_process.pid == completed_process.pid:
                original_process.completion_time = completed_process.completion_time
                original_process.turnaround_time = completed_process.turnaround_time
                original_process.waiting_time = completed_process.waiting_time
                break

    return gantt_chart

# Function to generate a Gantt chart graph
# Update the function to generate a Gantt chart graph with a title
def generate_gantt_chart(gantt_chart, title):
    fig, gnt = plt.subplots(figsize=(12, 6))

    # Setting labels and chart details
    gnt.set_xlabel('Time')
    gnt.set_ylabel('Processes')
    gnt.set_yticks([i + 1 for i in range(len(set(entry[0] for entry in gantt_chart)))])
    gnt.set_yticklabels([f'P{pid}' for pid in sorted(set(entry[0] for entry in gantt_chart))])
    gnt.grid(True)

    # Set the title
    plt.title(title)

    colors = plt.cm.get_cmap('Set3')(np.linspace(0, 1, len(set(entry[0] for entry in gantt_chart))))
    color_map = {pid: color for pid, color in zip(sorted(set(entry[0] for entry in gantt_chart)), colors)}

    for pid, start, end in gantt_chart:
        gnt.broken_barh([(start, end - start)], (pid, 0.8), facecolors=color_map[pid])

    plt.show()


# Function to display waiting time and turnaround time
def display_times(processes):
    print("\nProcess    Arrival   Burst     Priority    Finish Time   Wait Time    Turnaround")
    for process in processes:
        print(f"P{process.pid}\t\t{process.arrival_time}\t{process.burst_time}\t{process.priority}\t    {process.completion_time}\t\t  {process.waiting_time}\t     {process.turnaround_time}")

# Main function
def main():
    # Preset processes
    processes = [
              #ID,AT,BT,Pr
        Process(1, 2, 2,1),
        Process(2, 1, 1,3),
        Process(3, 0, 7,4),
        Process(4, 3, 5,2),
    ]

    # Set fixed time quantum for Round Robin
    quantum = 1

    print("Given Process Information:")
    print("ID\tArrival Time\tBurst Time\tPriority")
    for p in processes:
        print(f"{p.pid}\t{p.arrival_time}\t\t{p.burst_time}\t\t{p.priority}")
    print(f"\nTime Quantum for Round Robin: {quantum}")

    print("\n--- FCFS ---")
    gantt_chart_fcfs = fcfs(processes)
    display_times(processes)
    generate_gantt_chart(gantt_chart_fcfs, "FCFS Scheduling")

    print("\n--- Round Robin ---")
    gantt_chart_rr = round_robin(processes, quantum)
    display_times(processes)
    generate_gantt_chart(gantt_chart_rr, f"Round Robin Scheduling (Quantum = {quantum})")

    print("\n--- Non-preemptive SJF ---")
    gantt_chart_np_sjf = non_preemptive_sjf(processes)
    display_times(processes)
    generate_gantt_chart(gantt_chart_np_sjf, "Non-preemptive SJF Scheduling")

    print("\n--- Preemptive SJF ---")
    gantt_chart_p_sjf = preemptive_sjf(processes)
    display_times(processes)
    generate_gantt_chart(gantt_chart_p_sjf, "Preemptive SJF Scheduling")

    print("\n--- Non-preemptive Priority ---")
    gantt_chart_np_priority = non_preemptive_priority(processes)
    display_times(processes)
    generate_gantt_chart(gantt_chart_np_priority, "Non-preemptive Priority Scheduling")

    print("\n--- Preemptive Priority ---")
    gantt_chart_p_priority = preemptive_priority(processes)
    display_times(processes)
    generate_gantt_chart(gantt_chart_p_priority, "Preemptive Priority Scheduling")
if __name__ == "__main__":
    main()