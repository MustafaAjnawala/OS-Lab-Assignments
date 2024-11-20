def fifo_page_replacement(pages, frame_capacity):
    frames = []
    faults = 0
    record = []
    
    for page in pages:
        if page not in frames:
            if len(frames) < frame_capacity:
                frames.append(page)
            else:
                frames.pop(0)
                frames.append(page)
            faults += 1
            status = "Fault"
        else:
            status = "Hit"
        record.append((frames[:], status))
    
    print("FIFO Page Replacement Results")
    display_result(record, frame_capacity, faults, pages)

def lru_page_replacement(pages, frame_capacity):
    frames = []
    faults = 0
    record = []
    
    for idx, page in enumerate(pages):
        if page not in frames:
            if len(frames) < frame_capacity:
                frames.append(page)
            else:
                # Determine the least recently used page to replace
                lru_index = min(range(len(frames)), key=lambda x: pages[:idx].index(frames[x]))
                frames[lru_index] = page
            faults += 1
            status = "Fault"
        else:
            # Move the used page to the "most recent" position
            frames.remove(page)
            frames.append(page)
            status = "Hit"
        record.append((frames[:], status))
    
    print("LRU Page Replacement Results")
    display_result(record, frame_capacity, faults, pages)

def optimal_page_replacement(pages, frame_capacity):
    frames = []
    faults = 0
    record = []
    
    for idx in range(len(pages)):
        page = pages[idx]
        if page not in frames:
            if len(frames) < frame_capacity:
                frames.append(page)
            else:
                # Identify which page will not be needed for the longest time and replace it
                future_uses = [pages[idx+1:].index(f) if f in pages[idx+1:] else float('inf') for f in frames]
                to_replace = future_uses.index(max(future_uses))
                frames[to_replace] = page
            faults += 1
            status = "Fault"
        else:
            status = "Hit"
        record.append((frames[:], status))
    
    print("Optimal Page Replacement Results")
    display_result(record, frame_capacity, faults, pages)

def display_result(record, frame_capacity, faults, pages):
    # Display the page sequence header
    print("\n{:<8}".format("Page"), end="")
    for page in pages:
        print("{:^6}".format(page), end="")
    print()
    
    # Output frames' states at each step
    for frame_idx in range(frame_capacity):
        print("{:<8}".format("Frame " + str(frame_idx + 1)), end="")
        for frames, _ in record:
            if frame_idx < len(frames):
                print("{:^6}".format(frames[frame_idx]), end="")
            else:
                print("{:^6}".format(" "), end="")
        print()
    
    # Output Hit/Miss for each page reference
    print("{:<8}".format("Status"), end="")
    for _, status in record:
        print("{:^6}".format(status), end="")
    print("\n\nTotal Page Faults = {}\n".format(faults))

# Main program to choose an algorithm and run it
def main():
    while True:
        print("Select a Page Replacement Method:")
        print("1. FIFO (First In First Out)")
        print("2. LRU (Least Recently Used)")
        print("3. Optimal (Optimal Page Replacement)")
        print("4. Exit")
        
        choice = input("Enter your choice (1-4): ")
        if choice == '4':
            print("Program exiting.")
            break
        
        pages = list(map(int, input("Enter the page reference sequence (space-separated): ").split()))
        frame_capacity = int(input("Enter the number of frames: "))
        
        if choice == '1':
            fifo_page_replacement(pages, frame_capacity)
        elif choice == '2':
            lru_page_replacement(pages, frame_capacity)
        elif choice == '3':
            optimal_page_replacement(pages, frame_capacity)
        else:
            print("Invalid option. Please select a number between 1 and 4.")

if __name__ == "__main__":
    main()