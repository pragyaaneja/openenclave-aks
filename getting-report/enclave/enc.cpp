// Copyright (c) Open Enclave SDK contributors.
// Licensed under the MIT License.

#include <stdio.h>
#include <string.h>
#include <openenclave/attestation/sgx/evidence.h>
#include <openenclave/attestation/custom_claims.h>
#include <openenclave/attestation/verifier.h>
#include <openenclave/attestation/attester.h>
// #include <openenclave/internal/raise.h>

// Include the trusted getting_report header that is generated
// during the build. This file is generated by calling the
// sdk tool oeedger8r against the getting_report.edl file.
#include "getting_report_t.h"

// This is the function that the host calls. It tries
// to generate a report & fails if it cannot
int enclave_getting_report()
{

    static uint8_t* report;
    size_t report_size = 0;
    int ret = 1;

    // uint8_t** evidence;
    // size_t* evidence_size;
    uint8_t* evidence = NULL;
    size_t evidence_size = 0;
    static oe_uuid_t sgx_remote_uuid = {OE_FORMAT_UUID_SGX_ECDSA};
    static oe_uuid_t sgx_local_uuid = {OE_FORMAT_UUID_SGX_LOCAL_ATTESTATION};

    const oe_uuid_t* format_id = &sgx_remote_uuid;

    uint8_t* custom_claims_buffer = nullptr;
    size_t custom_claims_buffer_size = 0;
    uint8_t* format_settings = nullptr;
    size_t format_settings_size = 0;

    oe_result_t get_evidence_result = OE_OK;
    oe_result_t result = OE_OK;
    oe_result_t verify_evidence_result = OE_OK;

    result = oe_attester_initialize();
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_attester_initialize failed.\n");
        goto exit;
    }


    // Generate evidence.
    get_evidence_result = oe_get_evidence(
        format_id,
        // OE_EVIDENCE_FLAGS_EMBED_FORMAT_ID,
        0,
        custom_claims_buffer,
        custom_claims_buffer_size,
        // (format_settings_size == 0) ? nullptr : format_settings,
        format_settings,
        format_settings_size,
        &evidence,
        &evidence_size,
        nullptr,
        0);
    
    if (get_evidence_result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_get_evidence failed: result=%u (%s)\n",
            get_evidence_result,
            oe_result_str(get_evidence_result));
        goto exit;
    }
    
    printf(
        "oe_get_evidence passed: result=%u (%s)\n",
        get_evidence_result,
        oe_result_str(get_evidence_result));
    


    result = oe_verifier_initialize();
    if (result != OE_OK)
    {
        fprintf(
            stderr,
            "oe_verifier_initialize failed.\n");
        goto exit;
    }

    // Verify the evidence.
    verify_evidence_result = oe_verify_evidence(
        format_id,
        evidence,
        evidence_size,
        nullptr,
        0,
        nullptr,
        0,
        nullptr,
        0);

    if (verify_evidence_result != OE_OK) 
    {
        fprintf(
            stderr,
            "oe_verify_evidence failed: result=%u (%s)\n",
            verify_evidence_result,
            oe_result_str(verify_evidence_result));
        goto exit;
    }

     printf(
        "oe_verify_evidence passed: result=%u (%s)\n",
        verify_evidence_result,
        oe_result_str(verify_evidence_result));
    
    ret = 0;
    oe_free_evidence(evidence);
exit:
    return ret;
}