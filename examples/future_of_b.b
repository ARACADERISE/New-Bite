use std_allocation;

fun welcome_msg(usernames:Array<String>):Array<String> {
    welcomeMsgEach:Array<String> = b_calloc(1,size(*welcomeMsgEach));
    arraySize:Int = 0;

    usernames.forEach(i, {
        welcomeMsgEach[arraySize] = "Hello, {STRING}".format(i);

        arraySize++;
        welcomeMsgEach = b_realloc(
            welcomeMsgEach,
            arraySize*size(*welcomeMsgEach)
        );
    });

    ret welcomeMsgEach;
}