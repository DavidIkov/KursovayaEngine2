
!!!!!CONST FUNCTIONS IN CLASSES
for example you want to return some const Event& from some class with a function so other program will be able to connect to this event
but when program is connecting to event its modifying the actual event(its connections vector) so functions that are supposed to be possible
to call when variable returned and const reference should have modifier "const" in them, and so mutable variables will become a thing too


!!!!!NAMING CLASSES
when you have some class that have a name for example "Shader" you should name it "ShaderClass" instead becouse naming it just "Shader" will take away
possibility to name some variable of type "Shader" as "Shader", so classes that have not very specialized name should have "Class" at the end of a name

!!!!!PRIVATE CLASS FUNCTIONS
their name should start with "_" so for example _updSmth

!!!!! GET/SET FUNCTIONS IN CLASSES
if in order to get/set somnething some calculations have to be done or its not trivial then function name should start with Get/Set
but if its trivial then i can start with g/s instead


