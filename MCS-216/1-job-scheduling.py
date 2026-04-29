def sjf_minimize_time(job_ids: list[int], service_times: list[int]) -> tuple:
    """Schedule jobs using SJF to minimize total turnaround time (arrival=0 assumed)."""
    # 1. Pair & sort by service time (core SJF step)
    jobs = sorted(zip(job_ids, service_times), key=lambda x: x[1])

    current_time = 0
    total_tat = 0
    schedule = []

    # 2. Compute start, completion & turnaround sequentially
    for j_id, s_time in jobs:
        start = current_time
        completion = start + s_time
        tat = completion  # TAT = Completion - Arrival(0)
        schedule.append((j_id, s_time, start, completion, tat))
        total_tat += tat
        current_time = completion

    return schedule, total_tat


if __name__ == "__main__":
    # Assignment Data
    job_ids = [1, 5, 2, 10]
    service_times = [3, 7, 4, 8]

    # Execute & Print Compact Table
    schedule, total_tat = sjf_minimize_time(job_ids, service_times)
    print(f"{'Job':<4} {'Svc':<4} {'Start':<5} {'Comp':<5} {'TAT'}")
    for j in schedule:
        print(f"{j[0]:<4} {j[1]:<4} {j[2]:<5} {j[3]:<5} {j[4]}")
    print(f"\nTotal Time in System (ΣTAT): {total_tat} units")
