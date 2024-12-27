
!!!!!MUTABLE VARIABLES
mutable should mark variables that are not directly describing some state of object OR variables that are used only when KE2_Debug is defined(debug variables)

!!!!!NAMING CLASSES
when you have some class that have a name for example "Shader" you should name it "ShaderClass" instead becouse naming it just "Shader" will take away
possibility to name some variable of type "Shader" as "Shader", so classes that have not very specialized name should have "Class" at the end of a name

!!!!!PRIVATE CLASS FUNCTIONS
their name should start with "_" so for example _UpdSmth

!!!!! GET/SET FUNCTIONS IN CLASSES
if in order to get/set somnething some calculations have to be done or its not trivial then function name should start with Get/Set
but if its trivial then name can start with g/s instead

!!!!! ERRORS/WARNINGS
-warning happens when input is incorrect BUT can be fixed without any problems, warnings NEVER throw
-errors happen when input is incorrect and it cant be fixed without any problems, or output is not available, errors ALWAYS throw
-safety checks that check input happens ONLY in debug mode, in efficiency they are disabled, but safety checks that are working with
some third party software should happen on any config
-ALL errors should inherit from ErrorBase located at ErrorsSystem.h, so they could be obtained with just catch(AnyError& err)

