
!!!!!MUTABLE VARIABLES
mutable should mark variables that are responsible for class to work properly, like "Deleted" or stuff like that, that can also be usefull for debugging

!!!!!NAMING CLASSES
when you have some class that have a name for example "Shader" you should name it "ShaderClass" instead becouse naming it just "Shader" will take away
possibility to name some variable of type "Shader" as "Shader", so classes that have not very specialized name should have "Class" at the end of a name

!!!!!PRIVATE CLASS FUNCTIONS
their name should start with "_" so for example _UpdSmth

!!!!! GET/SET FUNCTIONS IN CLASSES
if in order to get/set somnething some calculations have to be done or its not trivial then function name should start with Get/Set
but if its trivial then name can start with g/s instead


