// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <openenclave/host.h>
#include <stdio.h>

#include "getting_report_u.h"

bool check_simulate_opt(int* argc, const char* argv[])
{
    for (int i = 0; i < *argc; i++)
    {
        if (strcmp(argv[i], "--simulate") == 0)
        {
            fprintf(stdout, "Running in simulation mode\n");
            memmove(&argv[i], &argv[i + 1], (*argc - i) * sizeof(char*));
            (*argc)--;
            return true;
        }
    }
    return false;
}

int main(int argc, const char* argv[])
{
    oe_result_t result;
    int ret = 1;
    oe_enclave_t* enclave = NULL;

    uint32_t flags = OE_ENCLAVE_FLAG_DEBUG;
    if (check_simulate_opt(&argc, argv))
    {
        flags |= OE_ENCLAVE_FLAG_SIMULATE;
    }

    if (argc != 2)
    {
        fprintf(
            stderr, "Usage: %s enclave_image_path [ --simulate  ]\n", argv[0]);
        goto exit;
    }

    // Create the enclave
    result = oe_create_getting_report_enclave(
        argv[1], OE_ENCLAVE_TYPE_AUTO, flags, NULL, 0, &enclave);
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_create_getting_report_enclave(): result=%u (%s)\n",
            result,
            oe_result_str(result));
        goto exit;
    }

    // Call into the enclave
    // result = enclave_getting_report(enclave);
    // if (result != OE_OK)
    // {
    //     fprintf(
    //         stderr,
    //         "calling into enclave_getting_report failed: result=%u (%s)\n",
    //         result,
    //         oe_result_str(result));
    //     goto exit;
    // }

    // ret = 0;


    // Call into the enclave
    result = enclave_getting_report(enclave, &ret);
    // fprintf(
    //         stderr,
    //         "THIS IS the value of ret = %d\n",
    //         ret);
    if ((result != OE_OK) || (ret != 0))
    {
        fprintf(
            stderr,
            "calling into enclave_getting_report failed: result=%u (%s)\n",
            result,
            oe_result_str(result));
        if (ret == 0)
            ret = 1;
        goto exit;
    }

    // fprintf(
    //         stderr,
    //         "THIS IS the final value of ret = %d\n",
    //         ret);
    ret = 0;

exit:
    // Clean up the enclave if we created one
    if (enclave)
        oe_terminate_enclave(enclave);

    return ret;
}
