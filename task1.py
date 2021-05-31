import queue
def read_pages(pages):
#     with open('data/pages_small.txt') as f:
    with open('data/pages.txt') as f:
        for data in f.read().splitlines():
            page = data.split('\t')
              # page[0]: id, page[1]: title
            pages[page[0]] = page[1]

def read_links(links):
#     with open('data/links_small.txt') as f:
    with open('data/links.txt') as f:
        for data in f.read().splitlines():
            link = data.split('\t')
              # link[0]: id (from), links[1]: id (to)
            if link[0] in links:
                links[link[0]].add(link[1])
            else:
                links[link[0]] = {link[1]}

#ページをIDに変換する
def search_ID(start_page, goal_page, pages):
    already_searched = 0
    start_ID = -1
    goal_ID = -1
    for ID, page in pages.items():
        if page == start_page:
            start_ID = ID
            already_searched += 1
            if already_searched==2:
                break
        if page == goal_page:
            goal_ID = ID
            already_searched += 1
            if already_searched == 2:
                break
                
    if already_searched != 2:
        raise ValueError("error!")
    return start_ID, goal_ID

#探すIDが見つかるまで幅優先探索を行う。見つかったらtrueを見つからなかったらfalseを返す
def bfs_goal_ID (start_ID, goal_ID, previous_ID, links):
    found = False
    q = queue.Queue()
    q.put(start_ID)
    while not q.empty():
        current_ID = q.get()
        if current_ID == goal_ID:
            found = True
            break;
        if  current_ID in links:    
            for next_ID in links[current_ID]:
                if next_ID in previous_ID:
                    continue
                previous_ID[next_ID] = current_ID
                q.put(next_ID)
    return found

# goalからstartまで辿っていき、途中で通るpageのIDをroute配列に入れて返す。
def reverse_route(start_ID, goal_ID, previous_ID, route):
    route.append(goal_ID)
    current_ID = goal_ID
    while 1:
        current_ID = previous_ID[current_ID]
        route.append(current_ID)
        if current_ID == start_ID:
            break
            
            
def print_route(route, pages):
    distance = len(route)
    for i in range(distance):
        if i!=distance-1:
            print(pages[route[distance-i-1]], end='->')
        else:
            print(pages[route[distance-i-1]])

if __name__ == '__main__':
    start_page, goal_page = input().split()
    pages = {}
    links = {}
    previous_ID = {}
    route = []
    read_pages(pages)
    read_links(links)
    start_ID, goal_ID = search_ID(start_page, goal_page, pages)
    if bfs_goal_ID(start_ID, goal_ID, previous_ID, links)==False:
        print("No Connection")
    else:
        reverse_route(start_ID, goal_ID, previous_ID, route)
        print_route(route, pages)
