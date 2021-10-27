
# The `generate_direct.py` script

## usage example

```
./generate_direct.py ../WORKSPACE/DOWNLOADED/casestudy/test.c direct \
                    " -I../WORKSPACE/DOWNLOADED/casestudy/" -c generate_template_config.json
```

## Configuration format

```yaml
{
    # Specify a type as key and the type conversion template as value, 
    # Where the placeholder for the expression to convert should be specified as the string '{}'
    # e.g. "TYPES_TO_INTCONVERT": {"flag": "(int){}"},
    "TYPES_TO_INTCONVERT": {},

    # Specify a type as key and the object printing code as value.
    # Where the placeholder for the object to print must be specified as the string '{}'
    # Do not forget to escape the backslashes and double quoted in printf fmt.
    # e.g. "TYPES_TO_PRINTCODE": {"struct XY *": "printf(\"FAQAS-SEMU-TEST_OUTPUT: X=%d, Y=%s\\n\", {}->x, {}->y);"}
    "TYPES_TO_PRINTCODE": {},

    # Specify the names of function arguments that are used as function output (passed by reference for output only)
    # e.g. "OUT_ARGS_NAMES": ["pErrCode"],
    "OUT_ARGS_NAMES": [],

    # Specify the names of function arguments that are used both as function input and output (passed by reference)
    # e.g. "IN_OUT_ARGS_NAMES": ["inoutArg"],
    "IN_OUT_ARGS_NAMES": [],

    # Specify a type as key and the pre 'klee_make_symbolic' statement initialization code as value,
    # The placeholder for the object to initialize must be specified as the string '{}'
    # e.g. "TYPE_TO_INITIALIZATIONCODE": {"struct head": "{}.next = malloc(sizeof(struct head));\n{}.next->next = NULL;"}
    "TYPE_TO_INITIALIZATIONCODE": {},

    # Specify how to make an object symbolic (specialy useful for objects that are initialized, like pointers).
    # The object type is the dict key and a dict of field accesses and their type is the dict value.
    # The placeholder for the object to make symbolic must be specified as the string '{}'
    # e.g. "TYPE_TO_SYMBOLIC_FIELDS_ACCESS": {"struct head": {"{}.data": "char [3]", "{}.next->data": "char [3]"}}
    # If you want an object not to be made symbolic (for example when completely initialized), 
    # specify the type here as key and with an empty dict as value
    "TYPE_TO_SYMBOLIC_FIELDS_ACCESS": {},

    # Specify the underlying type for a void pointer (the data type pointed by the void pointer).
    # for instance, if the function is to be called with an `int` array for a void pointer parameter,
    # set `VOID_ARG_SUBSTITUTE_TYPE` to "int".
    # Set the value to the empty string ("") to let the user specify at runtime on case by case
    # Set the value to `null` (JSON equivalent to None) to let the user change the types `void` 
    # directly in the generated templates
    # e.g. VOID_ARG_SUBSTITUTE_TYPE: "char"
    VOID_ARG_SUBSTITUTE_TYPE: "",

    # Specify, for pointer parameters, the number of elements it points to (must be > 0)
    # This will give the flexibility to set the number of elements the pointer points to.
    # The default value is `1`, for non specified types.
    # e.g. ARG_TYPE_TO_ITS_POINTER_ELEM_NUM: {"int *": 2, "char *": 6}
    # This let and array of 2 for int pointer and array of 6 for char pointer, 
    # and an array of 1 for unsigned pointer
    ARG_TYPE_TO_ITS_POINTER_ELEM_NUM: {}
}
```

check [generate_template_config.json](generate_template_config.json) for an example.
