/*
 *  X.509 certificate and private key decoding
 *
 *  Copyright (C) 2006-2011, Brainspark B.V.
 *
 *  This file is part of PolarSSL (http://www.polarssl.org)
 *  Lead Maintainer: Paul Bakker <polarssl_maintainer at polarssl.org>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 *  The ITU-T X.509 standard defines a certificat format for PKI.
 *
 *  http://www.ietf.org/rfc/rfc2459.txt
 *  http://www.ietf.org/rfc/rfc3279.txt
 *
 *  ftp://ftp.rsasecurity.com/pub/pkcs/ascii/pkcs-1v2.asc
 *
 *  http://www.itu.int/ITU-T/studygroups/com17/languages/X.680-0207.pdf
 *  http://www.itu.int/ITU-T/studygroups/com17/languages/X.690-0207.pdf
 */

#include "polarssl/config.h"

#if defined(POLARSSL_X509_PARSE_C)

#include "polarssl/x509.h"
#include "polarssl/asn1.h"
#include "polarssl/pem.h"
#include "polarssl/md2.h"
#include "polarssl/md4.h"
#include "polarssl/md5.h"
#include "polarssl/sha1.h"
#include "polarssl/sha2.h"
#include "polarssl/sha4.h"

#include <string.h>
#include <stdlib.h>
#if defined(_WIN32)
#include <windows.h>
#else
#include <time.h>
#endif

#if defined(POLARSSL_FS_IO)
#include <stdio.h>
#endif


/*
 *  AlgorithmIdentifier  ::=  SEQUENCE  {
 *       algorithm               OBJECT IDENTIFIER,
 *       parameters              ANY DEFINED BY algorithm OPTIONAL  }
 */
static int x509_get_alg( unsigned char **p,
                         const unsigned char *end,
                         x509_buf *alg )
{
    int ret;
    size_t len;

    if( ( ret = asn1_get_tag( p, end, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_ALG + ret );

    end = *p + len;
    alg->tag = **p;

    if( ( ret = asn1_get_tag( p, end, &alg->len, ASN1_OID ) ) != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_ALG + ret );

    alg->p = *p;
    *p += alg->len;

    if( *p == end )
        return( 0 );

    /*
     * assume the algorithm parameters must be NULL
     */
    if( ( ret = asn1_get_tag( p, end, &len, ASN1_NULL ) ) != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_ALG + ret );

    if( *p != end )
        return( POLARSSL_ERR_X509_CERT_INVALID_ALG +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );

    return( 0 );
}

/*
 *  SubjectPublicKeyInfo  ::=  SEQUENCE  {
 *       algorithm            AlgorithmIdentifier,
 *       subjectPublicKey     BIT STRING }
 */
static int x509_get_pubkey( unsigned char **p,
                            const unsigned char *end,
                            x509_buf *pk_alg_oid,
                            mpi *N, mpi *E )
{
    int ret, can_handle;
    size_t len;
    unsigned char *end2;

    if( ( ret = x509_get_alg( p, end, pk_alg_oid ) ) != 0 )
        return( ret );

    /*
     * only RSA public keys handled at this time
     */
    can_handle = 0;

    if( pk_alg_oid->len == 9 &&
        memcmp( pk_alg_oid->p, OID_PKCS1_RSA, 9 ) == 0 )
        can_handle = 1;

    if( pk_alg_oid->len == 9 &&
        memcmp( pk_alg_oid->p, OID_PKCS1, 8 ) == 0 )
    {
        if( pk_alg_oid->p[8] >= 2 && pk_alg_oid->p[8] <= 5 )
            can_handle = 1;

        if ( pk_alg_oid->p[8] >= 11 && pk_alg_oid->p[8] <= 14 )
            can_handle = 1;
    }

    if( pk_alg_oid->len == 5 &&
        memcmp( pk_alg_oid->p, OID_RSA_SHA_OBS, 5 ) == 0 )
        can_handle = 1;

    if( can_handle == 0 )
        return( POLARSSL_ERR_X509_UNKNOWN_PK_ALG );

    if( ( ret = asn1_get_tag( p, end, &len, ASN1_BIT_STRING ) ) != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY + ret );

    if( ( end - *p ) < 1 )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY +
                POLARSSL_ERR_ASN1_OUT_OF_DATA );

    end2 = *p + len;

    if( *(*p)++ != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY );

    /*
     *  RSAPublicKey ::= SEQUENCE {
     *      modulus           INTEGER,  -- n
     *      publicExponent    INTEGER   -- e
     *  }
     */
    if( ( ret = asn1_get_tag( p, end2, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY + ret );

    if( *p + len != end2 )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );

    if( ( ret = asn1_get_mpi( p, end2, N ) ) != 0 ||
        ( ret = asn1_get_mpi( p, end2, E ) ) != 0 )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY + ret );

    if( *p != end )
        return( POLARSSL_ERR_X509_CERT_INVALID_PUBKEY +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );

    return( 0 );
}



#if defined(POLARSSL_FS_IO)
/*
 * Load all data from a file into a given buffer.
 */
int load_file( const char *path, unsigned char **buf, size_t *n )
{
    FILE *f;

    if( ( f = fopen( path, "rb" ) ) == NULL )
        return( POLARSSL_ERR_X509_FILE_IO_ERROR );

    fseek( f, 0, SEEK_END );
    *n = (size_t) ftell( f );
    fseek( f, 0, SEEK_SET );

    if( ( *buf = (unsigned char *) malloc( *n + 1 ) ) == NULL )
        return( POLARSSL_ERR_X509_MALLOC_FAILED );

    if( fread( *buf, 1, *n, f ) != *n )
    {
        fclose( f );
        free( *buf );
        return( POLARSSL_ERR_X509_FILE_IO_ERROR );
    }

    fclose( f );

    (*buf)[*n] = '\0';

    return( 0 );
}

/*
 * Load and parse a private RSA key
 */
int x509parse_keyfile( rsa_context *rsa, const char *path, const char *pwd )
{
    int ret;
    size_t n;
    unsigned char *buf;

    if ( (ret = load_file( path, &buf, &n ) ) != 0 )
        return( ret );

    if( pwd == NULL )
        ret = x509parse_key( rsa, buf, n, NULL, 0 );
    else
        ret = x509parse_key( rsa, buf, n,
                (unsigned char *) pwd, strlen( pwd ) );

    memset( buf, 0, n + 1 );
    free( buf );

    return( ret );
}

/*
 * Load and parse a public RSA key
 */
int x509parse_public_keyfile( rsa_context *rsa, const char *path )
{
    int ret;
    size_t n;
    unsigned char *buf;

    if ( (ret = load_file( path, &buf, &n ) ) != 0 )
        return( ret );

    ret = x509parse_public_key( rsa, buf, n );

    memset( buf, 0, n + 1 );
    free( buf );

    return( ret );
}
#endif /* POLARSSL_FS_IO */

/*
 * Parse a private RSA key
 */
int x509parse_key( rsa_context *rsa, const unsigned char *key, size_t keylen,
                                     const unsigned char *pwd, size_t pwdlen )
{
    int ret;
    size_t len;
    unsigned char *p, *end;
    unsigned char *p_alt;
    x509_buf pk_alg_oid;

#if defined(POLARSSL_PEM_C)
    pem_context pem;

    pem_init( &pem );
    ret = pem_read_buffer( &pem,
                           "-----BEGIN RSA PRIVATE KEY-----",
                           "-----END RSA PRIVATE KEY-----",
                           key, pwd, pwdlen, &len );

    if( ret == POLARSSL_ERR_PEM_NO_HEADER_PRESENT )
    {
        ret = pem_read_buffer( &pem,
                           "-----BEGIN PRIVATE KEY-----",
                           "-----END PRIVATE KEY-----",
                           key, pwd, pwdlen, &len );
    }

    if( ret == 0 )
    {
        /*
         * Was PEM encoded
         */
        keylen = pem.buflen;
    }
    else if( ret != POLARSSL_ERR_PEM_NO_HEADER_PRESENT )
    {
        pem_free( &pem );
        return( ret );
    }

    p = ( ret == 0 ) ? pem.buf : (unsigned char *) key;
#else
    ((void) pwd);
    ((void) pwdlen);
    p = (unsigned char *) key;
#endif
    end = p + keylen;

    /*
     * Note: Depending on the type of private key file one can expect either a
     * PrivatKeyInfo object (PKCS#8) or a RSAPrivateKey (PKCS#1) directly.
     *
     *  PrivateKeyInfo ::= SEQUENCE {
     *    version           Version,
     *    algorithm       AlgorithmIdentifier,
     *    PrivateKey      BIT STRING
     *  }
     *
     *  AlgorithmIdentifier ::= SEQUENCE {
     *    algorithm       OBJECT IDENTIFIER,
     *    parameters      ANY DEFINED BY algorithm OPTIONAL
     *  }
     *
     *  RSAPrivateKey ::= SEQUENCE {
     *      version           Version,
     *      modulus           INTEGER,  -- n
     *      publicExponent    INTEGER,  -- e
     *      privateExponent   INTEGER,  -- d
     *      prime1            INTEGER,  -- p
     *      prime2            INTEGER,  -- q
     *      exponent1         INTEGER,  -- d mod (p-1)
     *      exponent2         INTEGER,  -- d mod (q-1)
     *      coefficient       INTEGER,  -- (inverse of q) mod p
     *      otherPrimeInfos   OtherPrimeInfos OPTIONAL
     *  }
     */
    if( ( ret = asn1_get_tag( &p, end, &len,
            ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
    }

    end = p + len;

    if( ( ret = asn1_get_int( &p, end, &rsa->ver ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
    }

    if( rsa->ver != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_KEY_INVALID_VERSION + ret );
    }

    p_alt = p;

    if( ( ret = x509_get_alg( &p_alt, end, &pk_alg_oid ) ) != 0 )
    {
        // Assume that we have the PKCS#1 format if wrong
        // tag was encountered
        //
        if( ret != POLARSSL_ERR_X509_CERT_INVALID_ALG +
                    POLARSSL_ERR_ASN1_UNEXPECTED_TAG )
        {
#if defined(POLARSSL_PEM_C)
            pem_free( &pem );
#endif
            rsa_free( rsa );
            return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT );
        } 
    }
    else
    {
        int can_handle;

        /*
         * only RSA keys handled at this time
         */
        can_handle = 0;

        if( pk_alg_oid.len == 9 &&
                memcmp( pk_alg_oid.p, OID_PKCS1_RSA, 9 ) == 0 )
            can_handle = 1;

        if( pk_alg_oid.len == 9 &&
                memcmp( pk_alg_oid.p, OID_PKCS1, 8 ) == 0 )
        {
            if( pk_alg_oid.p[8] >= 2 && pk_alg_oid.p[8] <= 5 )
                can_handle = 1;

            if ( pk_alg_oid.p[8] >= 11 && pk_alg_oid.p[8] <= 14 )
                can_handle = 1;
        }

        if( pk_alg_oid.len == 5 &&
                memcmp( pk_alg_oid.p, OID_RSA_SHA_OBS, 5 ) == 0 )
            can_handle = 1;

        if( can_handle == 0 )
            return( POLARSSL_ERR_X509_UNKNOWN_PK_ALG );

        /*
         * Parse the PKCS#8 format
         */
        
        p = p_alt;
        if( ( ret = asn1_get_tag( &p, end, &len, ASN1_OCTET_STRING ) ) != 0 )
        {
#if defined(POLARSSL_PEM_C)
            pem_free( &pem );
#endif
            rsa_free( rsa );
            return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
        }

        if( ( end - p ) < 1 )
        {
#if defined(POLARSSL_PEM_C)
            pem_free( &pem );
#endif
            rsa_free( rsa );
            return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT +
                    POLARSSL_ERR_ASN1_OUT_OF_DATA );
        }

        end = p + len;

        if( ( ret = asn1_get_tag( &p, end, &len,
                        ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
        {
#if defined(POLARSSL_PEM_C)
            pem_free( &pem );
#endif
            rsa_free( rsa );
            return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
        }

        end = p + len;

        if( ( ret = asn1_get_int( &p, end, &rsa->ver ) ) != 0 )
        {
#if defined(POLARSSL_PEM_C)
            pem_free( &pem );
#endif
            rsa_free( rsa );
            return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
        }

        if( rsa->ver != 0 )
        {
#if defined(POLARSSL_PEM_C)
            pem_free( &pem );
#endif
            rsa_free( rsa );
            return( POLARSSL_ERR_X509_KEY_INVALID_VERSION + ret );
        }
    }

    if( ( ret = asn1_get_mpi( &p, end, &rsa->N  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->E  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->D  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->P  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->Q  ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->DP ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->DQ ) ) != 0 ||
        ( ret = asn1_get_mpi( &p, end, &rsa->QP ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
    }

    rsa->len = mpi_size( &rsa->N );

    if( p != end )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT +
                POLARSSL_ERR_ASN1_LENGTH_MISMATCH );
    }

    if( ( ret = rsa_check_privkey( rsa ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( ret );
    }

#if defined(POLARSSL_PEM_C)
    pem_free( &pem );
#endif

    return( 0 );
}

/*
 * Parse a public RSA key
 */
int x509parse_public_key( rsa_context *rsa, const unsigned char *key, size_t keylen )
{
    int ret;
    size_t len;
    unsigned char *p, *end;
    x509_buf alg_oid;
#if defined(POLARSSL_PEM_C)
    pem_context pem;

    pem_init( &pem );
    ret = pem_read_buffer( &pem,
            "-----BEGIN PUBLIC KEY-----",
            "-----END PUBLIC KEY-----",
            key, NULL, 0, &len );

    if( ret == 0 )
    {
        /*
         * Was PEM encoded
         */
        keylen = pem.buflen;
    }
    else if( ret != POLARSSL_ERR_PEM_NO_HEADER_PRESENT )
    {
        pem_free( &pem );
        return( ret );
    }

    p = ( ret == 0 ) ? pem.buf : (unsigned char *) key;
#else
    p = (unsigned char *) key;
#endif
    end = p + keylen;

    /*
     *  PublicKeyInfo ::= SEQUENCE {
     *    algorithm       AlgorithmIdentifier,
     *    PublicKey       BIT STRING
     *  }
     *
     *  AlgorithmIdentifier ::= SEQUENCE {
     *    algorithm       OBJECT IDENTIFIER,
     *    parameters      ANY DEFINED BY algorithm OPTIONAL
     *  }
     *
     *  RSAPublicKey ::= SEQUENCE {
     *      modulus           INTEGER,  -- n
     *      publicExponent    INTEGER   -- e
     *  }
     */

    if( ( ret = asn1_get_tag( &p, end, &len,
                    ASN1_CONSTRUCTED | ASN1_SEQUENCE ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_CERT_INVALID_FORMAT + ret );
    }

    if( ( ret = x509_get_pubkey( &p, end, &alg_oid, &rsa->N, &rsa->E ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( POLARSSL_ERR_X509_KEY_INVALID_FORMAT + ret );
    }

    if( ( ret = rsa_check_pubkey( rsa ) ) != 0 )
    {
#if defined(POLARSSL_PEM_C)
        pem_free( &pem );
#endif
        rsa_free( rsa );
        return( ret );
    }

    rsa->len = mpi_size( &rsa->N );

#if defined(POLARSSL_PEM_C)
    pem_free( &pem );
#endif

    return( 0 );
}

#endif
