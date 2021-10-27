
Transform generated ktest into unittest by:
1. executing (using muteria) the generated ktest with the program with specialized annotated printf output (will print both parameters and output).
2. extracting both input and expected output, as key-value pairs, from the muteria test execution output (based on expected pattern).
3. Take the unittest jinja2 template and apply the extracted values into it.
