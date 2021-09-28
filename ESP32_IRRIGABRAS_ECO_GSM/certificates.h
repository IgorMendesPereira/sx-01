#ifndef _CERTIFICATES_H_
#define _CERTIFICATES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* This file is auto-generated by the pycert_bearssl tool.  Do not change it manually.
 * Certificates are BearSSL br_x509_trust_anchor format.  Included certs:
 *
 * Index:    0
 * Label:    Starfield Services Root Certificate Authority - G2
 * Subject:  CN=Starfield Services Root Certificate Authority - G2,O=Starfield Technologies\, Inc.,L=Scottsdale,ST=Arizona,C=US
 * Type:     Certificate Authority
 */

#define TAs_NUM 1

static const unsigned char TA_DN0[] = {
    0x30, 0x81, 0x98, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06,
    0x13, 0x02, 0x55, 0x53, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x13, 0x07, 0x41, 0x72, 0x69, 0x7a, 0x6f, 0x6e, 0x61, 0x31, 0x13,
    0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x07, 0x13, 0x0a, 0x53, 0x63, 0x6f,
    0x74, 0x74, 0x73, 0x64, 0x61, 0x6c, 0x65, 0x31, 0x25, 0x30, 0x23, 0x06,
    0x03, 0x55, 0x04, 0x0a, 0x13, 0x1c, 0x53, 0x74, 0x61, 0x72, 0x66, 0x69,
    0x65, 0x6c, 0x64, 0x20, 0x54, 0x65, 0x63, 0x68, 0x6e, 0x6f, 0x6c, 0x6f,
    0x67, 0x69, 0x65, 0x73, 0x2c, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31, 0x3b,
    0x30, 0x39, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x32, 0x53, 0x74, 0x61,
    0x72, 0x66, 0x69, 0x65, 0x6c, 0x64, 0x20, 0x53, 0x65, 0x72, 0x76, 0x69,
    0x63, 0x65, 0x73, 0x20, 0x52, 0x6f, 0x6f, 0x74, 0x20, 0x43, 0x65, 0x72,
    0x74, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x41, 0x75, 0x74,
    0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x20, 0x2d, 0x20, 0x47, 0x32,
};

static const unsigned char TA_RSA_N0[] = {
    0xd5, 0x0c, 0x3a, 0xc4, 0x2a, 0xf9, 0x4e, 0xe2, 0xf5, 0xbe, 0x19, 0x97,
    0x5f, 0x8e, 0x88, 0x53, 0xb1, 0x1f, 0x3f, 0xcb, 0xcf, 0x9f, 0x20, 0x13,
    0x6d, 0x29, 0x3a, 0xc8, 0x0f, 0x7d, 0x3c, 0xf7, 0x6b, 0x76, 0x38, 0x63,
    0xd9, 0x36, 0x60, 0xa8, 0x9b, 0x5e, 0x5c, 0x00, 0x80, 0xb2, 0x2f, 0x59,
    0x7f, 0xf6, 0x87, 0xf9, 0x25, 0x43, 0x86, 0xe7, 0x69, 0x1b, 0x52, 0x9a,
    0x90, 0xe1, 0x71, 0xe3, 0xd8, 0x2d, 0x0d, 0x4e, 0x6f, 0xf6, 0xc8, 0x49,
    0xd9, 0xb6, 0xf3, 0x1a, 0x56, 0xae, 0x2b, 0xb6, 0x74, 0x14, 0xeb, 0xcf,
    0xfb, 0x26, 0xe3, 0x1a, 0xba, 0x1d, 0x96, 0x2e, 0x6a, 0x3b, 0x58, 0x94,
    0x89, 0x47, 0x56, 0xff, 0x25, 0xa0, 0x93, 0x70, 0x53, 0x83, 0xda, 0x84,
    0x74, 0x14, 0xc3, 0x67, 0x9e, 0x04, 0x68, 0x3a, 0xdf, 0x8e, 0x40, 0x5a,
    0x1d, 0x4a, 0x4e, 0xcf, 0x43, 0x91, 0x3b, 0xe7, 0x56, 0xd6, 0x00, 0x70,
    0xcb, 0x52, 0xee, 0x7b, 0x7d, 0xae, 0x3a, 0xe7, 0xbc, 0x31, 0xf9, 0x45,
    0xf6, 0xc2, 0x60, 0xcf, 0x13, 0x59, 0x02, 0x2b, 0x80, 0xcc, 0x34, 0x47,
    0xdf, 0xb9, 0xde, 0x90, 0x65, 0x6d, 0x02, 0xcf, 0x2c, 0x91, 0xa6, 0xa6,
    0xe7, 0xde, 0x85, 0x18, 0x49, 0x7c, 0x66, 0x4e, 0xa3, 0x3a, 0x6d, 0xa9,
    0xb5, 0xee, 0x34, 0x2e, 0xba, 0x0d, 0x03, 0xb8, 0x33, 0xdf, 0x47, 0xeb,
    0xb1, 0x6b, 0x8d, 0x25, 0xd9, 0x9b, 0xce, 0x81, 0xd1, 0x45, 0x46, 0x32,
    0x96, 0x70, 0x87, 0xde, 0x02, 0x0e, 0x49, 0x43, 0x85, 0xb6, 0x6c, 0x73,
    0xbb, 0x64, 0xea, 0x61, 0x41, 0xac, 0xc9, 0xd4, 0x54, 0xdf, 0x87, 0x2f,
    0xc7, 0x22, 0xb2, 0x26, 0xcc, 0x9f, 0x59, 0x54, 0x68, 0x9f, 0xfc, 0xbe,
    0x2a, 0x2f, 0xc4, 0x55, 0x1c, 0x75, 0x40, 0x60, 0x17, 0x85, 0x02, 0x55,
    0x39, 0x8b, 0x7f, 0x05,
};

static const unsigned char TA_RSA_E0[] = {
    0x01, 0x00, 0x01,
};

static const br_x509_trust_anchor TAs[] = {
    {
        { (unsigned char *)TA_DN0, sizeof TA_DN0 },
        BR_X509_TA_CA,
        {
            BR_KEYTYPE_RSA,
            { .rsa = {
                (unsigned char *)TA_RSA_N0, sizeof TA_RSA_N0,
                (unsigned char *)TA_RSA_E0, sizeof TA_RSA_E0,
            } }
        }
    },
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ifndef _CERTIFICATES_H_ */
