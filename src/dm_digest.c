/*
 Copyright (C) 2007 NFG Net Facilities Group support@nfg.nl

 This program is free software; you can redistribute it and/or 
 modify it under the terms of the GNU General Public License 
 as published by the Free Software Foundation; either 
 version 2 of the License, or (at your option) any later 
 version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "dbmail.h"
#define THIS_MODULE "digest"


static char * dm_digest(const unsigned char * hash, hashid type)
{
	static int bufno;
	static char hexbuffer[8][128];
	static const char hex[] = "0123456789abcdef";
	char *buffer = hexbuffer[7 & ++bufno], *buf = buffer;
	size_t i, j;

	for (i = 0; i < mhash_get_block_size(type); i++) {
		j = i;
		if (type == MHASH_TIGER) {
			/* compensate for endian-ess */
			if (i<8) j=7-i;
			else if (i<16) j=23-i;
			else j=39-i;
		}
		unsigned int val = hash[j];
		*buf++ = hex[val >> 4];
		*buf++ = hex[val & 0xf];
	}
	*buf = '\0';

	return buffer;
}

static const unsigned char * dm_hash(const unsigned char * buf, hashid type)
{
	MHASH td = mhash_init(type);
	mhash(td, buf, strlen((const char *)buf));
	return (const unsigned char *)mhash_end(td);
}

char * dm_whirlpool(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);

	return dm_digest(dm_hash((unsigned char *)s, MHASH_WHIRLPOOL), MHASH_WHIRLPOOL);
}

char * dm_sha512(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);

	return dm_digest(dm_hash((unsigned char *)s, MHASH_SHA512), MHASH_SHA512);
}

char * dm_sha256(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);

	return dm_digest(dm_hash((unsigned char *)s, MHASH_SHA256), MHASH_SHA256);
}

char * dm_sha1(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);

	return dm_digest(dm_hash((unsigned char *)s, MHASH_SHA1), MHASH_SHA1);
}

char * dm_tiger(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);
	
	return dm_digest(dm_hash((unsigned char *)s, MHASH_TIGER), MHASH_TIGER);
}

char *dm_md5(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);

	return dm_digest(dm_hash((unsigned char *)s, MHASH_MD5), MHASH_MD5);
}

char *dm_md5_base64(const char * const s)
{
	g_return_val_if_fail(s != NULL, NULL);

	const unsigned char *h = (unsigned char *)dm_hash((unsigned char *)s, MHASH_MD5);

	return g_base64_encode(h, sizeof(h));
}

