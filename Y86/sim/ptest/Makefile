SIM=../pipe/psim
TFLAGS=

ISADIR = ../misc
YAS=$(ISADIR)/yas

.SUFFIXES: .ys .yo

.ys.yo:
	$(YAS) $*.ys

test:
	./optest.pl -i $(SIM) $(TFLAGS) # 9/58
	./jtest.pl -i $(SIM) $(TFLAGS) # 32/96
	./ctest.pl -i $(SIM) $(TFLAGS) #
	./htest.pl -i $(SIM) $(TFLAGS) #
	./optest.pl -l $(SIM) $(TFLAGS) # 1/50
	./jtest.pl -l $(SIM) $(TFLAGS)  #
	./ctest.pl -l $(SIM) $(TFLAGS) #
	./htest.pl -l $(SIM) $(TFLAGS)  #

clean:
	rm -f *.o *~ *.yo *.ys
