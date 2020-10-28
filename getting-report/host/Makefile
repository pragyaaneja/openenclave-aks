# Copyright (c) Open Enclave SDK contributors.
# Licensed under the MIT License.


include ../config.mk

CFLAGS=$(shell pkg-config oehost-$(COMPILER) --cflags)
LDFLAGS=$(shell pkg-config oehost-$(COMPILER) --libs)
INCDIR=$(shell pkg-config oehost-$(COMPILER) --variable=includedir)

build:
	@ echo "Compilers used: $(CC), $(CXX)"
	oeedger8r ../getting_report.edl --untrusted \
		--search-path $(INCDIR) \
		--search-path $(INCDIR)/openenclave/edl/sgx
	$(CC) -g -c $(CFLAGS) host.c
	$(CC) -g -c $(CFLAGS) getting_report_u.c
	$(CC) -o getting_reporthost getting_report_u.o host.o $(LDFLAGS)

clean:
	rm -f getting_reporthost host.o getting_report_u.o getting_report_u.c getting_report_u.h getting_report_args.h
