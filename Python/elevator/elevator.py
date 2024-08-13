MAX_LOAD = 4

class ElevatorState():
    def __init__(self, floor, number, direction, destination, user_destination):
        self.floor = floor
        self.number = number
        self.direction = direction
        self.destination = destination
        self.user_destination = user_destination

class Task():
    def __init__(self, user_floor, destination):
        self.user_floor = user_floor
        self.destination = destination

to_be_done = []

def create_by_hand():
    head = []
    print("Please enter the call time, current floor, and target floor respectively")
    data = input("The three are separated by a space,a value of 0 for all three indicates the end\n")
    call_time, user_floor, destination = map(int, data.split())
    while user_floor != 0:
        if call_time == 0:
            head.append(Task(user_floor, destination))
        else:
            to_be_done.append([call_time, user_floor, destination])
        call_time, user_floor, destination = map(int, input().split())
    return head

def create_by_file(f, initial_floor):
    head = []
    _, initial_floor = f.readline().split()
    for line in f:
        call_time, user_floor, destination = map(int, line.rstrip().split())
        if call_time == 0:
            head.append(Task(user_floor, destination))
        else:
            to_be_done.append([call_time, user_floor, destination])
    return head, int(initial_floor)

def set_direction(head, elevator_state):
    for destination in elevator_state.user_destination:
        if destination != 0:
            elevator_state.direction = destination - elevator_state.floor
            return
    if len(head) != 0:
        elevator_state.direction = head[0].user_floor - elevator_state.floor
    else:
        elevator_state.direction = 0

def set_destination(head, elevator_state):
    if elevator_state.direction > 0:
        for destination in elevator_state.user_destination:
            if destination > elevator_state.destination:
                elevator_state.destination = destination
        for p in head:
            if p.user_floor > elevator_state.destination:
                elevator_state.destination = p.user_floor
    elif elevator_state.direction < 0:
        for destination in elevator_state.user_destination:
            if destination != 0:
                if destination < elevator_state.destination:
                    elevator_state.destination = destination
        for p in head:
            if p.user_floor < elevator_state.destination:
                elevator_state.destination = p.user_floor

def if_exist(array, target):
    for i in range(MAX_LOAD):
        if array[i] == target:
            return i
    return -1

def determine_if_syntropy(direction, p):
    if direction > 0 and p.user_floor - p.destination < 0:
        return True
    elif direction < 0 and p.user_floor - p.destination > 0:
        return True
    return False

def add_task(head, run_time):
    to_be_removed = []
    for e in to_be_done:
        if e[0] == run_time:
            head.append(Task(e[1], e[2]))
            to_be_removed.append(e)
    for e in to_be_removed:
        to_be_done.remove(e)

def schedule_elevator(head, elevator_state):
    if_empty, if_end, run_time = True, True, 0
    for e in to_be_done:
        if e[0] != 0:
            if_end = False
            break
    while not (if_end and if_empty and len(head) == 0):
        # to_be_done不为空或者电梯不为空或者任务列表不为空
        index = if_exist(elevator_state.user_destination, elevator_state.floor)
        while index != -1:
            # 有人需要下电梯
            elevator_state.user_destination[index] = 0 # 下电梯
            elevator_state.number -= 1 # 人数减一
            index = if_exist(elevator_state.user_destination, elevator_state.floor)
        to_be_removed = []
        for p in head:
            if p.user_floor == elevator_state.floor:
                # 有人需要上电梯
                if_syntropy = determine_if_syntropy(elevator_state.direction, p)
                if if_syntropy or p.user_floor == elevator_state.destination:
                    # 目的方向与电梯方向相同
                    # 或者当前楼层为目的楼层
                    index = if_exist(elevator_state.user_destination, 0)
                    if index == -1:
                        # 超载
                        print(f"The elevator is overloaded on the {elevator_state.floor} floor")
                    else:
                        elevator_state.number += 1
                        elevator_state.user_destination[index] = p.destination # 上电梯
                        to_be_removed.append(p)
        for p in to_be_removed:
            head.remove(p)
        print(f"{run_time}\t{elevator_state.floor}\t\t{elevator_state.number}")
        set_direction(head, elevator_state)
        set_destination(head, elevator_state)
        elevator_state.floor = elevator_state.floor + (elevator_state.direction // abs(elevator_state.direction) if elevator_state.direction != 0 else 0) # 电梯朝给定方向移动一层
        run_time += 1
        add_task(head, run_time)
        for destination in elevator_state.user_destination:
            if destination != 0:
                if_empty = False
                break
            if_empty = True
        if_end = True
        for e in to_be_done:
            if e[0] != 0:
                if_end = False
                break

def main():
    print("Please enter a number to select the data entry method:")
    method = int(input("0:manual input   1:file input\n"))
    elevator_state = ElevatorState(1, 0, 0, 1, [0, 0, 0, 0])
    head = None
    if method == 0:
        elevator_state.floor = int(input("Please enter the initial floor:"))
        elevator_state.destination = elevator_state.floor
        head = create_by_hand()
    elif method == 1:
        file_name = r"Python\elevator\test.txt"
        with open(file_name) as f:
            head, elevator_state.floor = create_by_file(f, elevator_state.floor)
        elevator_state.destination = elevator_state.floor
    set_direction(head, elevator_state)
    set_destination(head, elevator_state)
    print("runTime\tcurrentFloor\tcurrentNumber")
    schedule_elevator(head, elevator_state)

if __name__ == "__main__":
    main()