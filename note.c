want the alien to randomly come down. after its done return to the original
position // need to save location

each alian needs two rects draw rect1 even if its not there
rect2 is alien
alien slowly moves back to origional position
rect needs to have a state moving in fleet
make a state diagram
state =  0 in fleet
state = 1 means attack -----> // rand() % 1000 == 0
state = 2 return to fleet --> // alien.rect.y >= H
                              // rect == fleet rect

this design method is call
fianite state diagram
or antomata

