#CFLAGS=-g
#CC=clang

run_with_timeout: run_with_timeout.c

.PHONY: test

test: run_with_timeout
	@echo
	@echo "Testcase 1"
	./run_with_timeout 1 'echo begin; sleep 2; echo end' || true
	@echo
	@echo "Testcase 2"
	./run_with_timeout 2 'echo begin; sleep 2; echo end' || true
	@echo
	@echo "Testcase 3"
	./run_with_timeout 3 'echo begin; sleep 2; echo end' || true
