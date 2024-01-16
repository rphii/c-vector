- when shrinking, add handle KEEP_ZERO_END !!!
  - -> and then add the NICE example to c-string
    str_resize(&string, 5);
    printf("after resize : %s\n", string.s); /* no change; "_dlXroW ,oYle" */
    printf("string length : %zu\n", str_length(&string)); /* 13 */

