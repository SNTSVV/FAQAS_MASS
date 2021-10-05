
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
    # e.g. "TYPES_TO_PRINTCODE": {"struct XY *": "printf(\"FAQAS-SEMU-TEST_OUTPUT: X=%d, Y=%s\\n\", {}->x, {}->y)"}
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
    "TYPE_TO_SYMBOLIC_FIELDS_ACCESS": {}
}
```

check [generate_template_config.json](generate_template_config.json) for an example.
