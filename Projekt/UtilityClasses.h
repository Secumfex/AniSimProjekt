/**
 * Allgemein Nützliche Klassen
 */
#ifndef UTILITYCLASSES_H
#define UTILITYCLASSES_H

/*
 * Klasse IntTupel fasst zwei Int-Werte zusammen.
 * Vglbar mit Tupel einer Relation
 */
class IntTupel{

public:
	int i;
	int j;
	IntTupel(int i, int j){
		this->i = i;
		this->j = j;
	}
};

#endif
