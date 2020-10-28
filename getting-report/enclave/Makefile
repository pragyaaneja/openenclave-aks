# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.

include ../config.mk

ifeq ($(LVI_MITIGATION), ControlFlow)
	ifeq ($(LVI_MITIGATION_BINDIR),)
		$(error LVI_MITIGATION_BINDIR is not set)
	endif
	# Only run once.
	ifeq (,$(findstring $(LVI_MITIGATION_BINDIR),$(CC)))
		CC := $(LVI_MITIGATION_BINDIR)/$(CC)
	endif
	COMPILER := $(COMPILER)-lvi-cfg
endif

CFLAGS=$(shell pkg-config oeenclave-$(COMPILER) --cflags)
LDFLAGS=$(shell pkg-config oeenclave-$(COMPILER) --libs)
INCDIR=$(shell pkg-config oeenclave-$(COMPILER) --variable=includedir)

all:
	$(MAKE) build
	$(MAKE) keys
	$(MAKE) sign

build:
	@ echo "Compilers used: $(CC), $(CXX)"
	oeedger8r ../getting_report.edl --trusted \
		--search-path $(INCDIR) \
		--search-path $(INCDIR)/openenclave/edl/sgx
	$(CC) -g -c $(CFLAGS) -DOE_API_VERSION=2 enc.c -o enc.o
	$(CC) -g -c $(CFLAGS) -DOE_API_VERSION=2 getting_report_t.c -o getting_report_t.o
	$(CC) -o getting_reportenc getting_report_t.o enc.o $(LDFLAGS)

sign:
	oesign sign -e getting_reportenc -c getting_report.conf -k private.pem

clean:
	rm -f enc.o getting_reportenc getting_reportenc.signed private.pem public.pem getting_report_t.o getting_report_t.h getting_report_t.c getting_report_args.h

keys:
	openssl genrsa -out private.pem -3 3072
	openssl rsa -in private.pem -pubout -out public.pem
