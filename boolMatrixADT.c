//boolMatrixADT.c

//Matriz booleana infinita, al menos 90% true :: por defecto todas las posiciones tienen TRUE (esto fácilmente se logra con calloc)
//Pienso a setTrue como un delete (si es que existe) 
//Pienso a setFalse como un add 


typedef struct tCol { 
	unsigned long col; 
	struct tCol * tail; 
} tCol; 

typedef struct tRow { 
	unsigned long row; 
	tCol * firstCol; 
	struct tRow * tail; 
} tRow; 

typedef struct boolMatrixCDT { 
	tRow * firstRow; 
} boolMatrixCDT; 

boolMatrixADT newMatrix() { 
	return calloc(1, sizeof(boolMatrixCDT)); 
}

void freeCols(tCol * col) { 
	if (col == NULL) 
		return; 
	freeCols(col->tail); 
	free(col); 
}

void freeRows(tRow * row) { 
	if (row == NULL) 
		return; 
	freeRows(row->tail); 
	freeCols(row->firstCol); 
	free(row); 
}


void freeBoolMatrix(boolMatrixADT m) { 
	freeRec(m->firstRow); 
	free(m); 
}

static tCol * deleteCol(tCol * first, unsigned long col) { 
	if (first == NULL || first->col > col) 
		return first; 
	if (first->col == col) {
		tCol * aux = first->tail; 
		free(first); 
		return aux; 
	}
		
	first->tail = deleteCol(first->tail, col); 
	return first; 
} 

tRow * deleteRow(tRow * first, unsigned long row, unsigned long col) {
	if (first == NULL || first->row>row) 
		return first; 
	if (first->row == row) {
		first->firstCol = deleteCol(first->firstCol, unsigned long col);
		if (first->firstCol == NULL) {	
			tRow * aux = first->tail; 
			free(first); 
			return aux; 
		} 
	}
	first->tail = deleteRow(first->tail, row, col); 
	return first; 
}

// delete 
void setTrue(boolMatrixADT m, unsigned long row, unsigned long col) {
	m->firstRow = deleteRow(m->firstRow, row, col);	 
}


//si no existe row y col lo puedo agregar 
// si existe row pero no col lo puedo agregar 
// para estas primeras dos opciones tengo que hacer un malloc 
// si existe row y col 


tCol * addCol(tCol * first, unsigned long col) { 
	if (first == NULL || col < first->col) { // si no hay columnas en el row en el que estamos, o si es el menor elemento, tengo que agregar la col
		tCol * aux = malloc(sizeof(*aux)); 
		aux->col = col; 
		aux->tail = first; 
		return aux; 
	}
	if (first->col < col) // ya existe entonces no agrego
		first->tail = addCol(first->tail, col); 
	// si ya existe no agrego 
	return first; 
}

tRow * addRow(tRow * first, unsigned long row, unsigned long col) { 
	if (first == NULL || row < first->row) {
		tRow * aux = malloc(sizeof(*aux)); 
		aux->row = row; 
		aux->tail= first; 
		aux->firstCol = addCol(aux->firstCol, row, col); 
		return aux; 
	}
	if (first->row < row) 
		first->tail = addRow(first->tail, row, col); 
	return first;
}

void setFalse (boolMatrixADT m, unsigned long row, unsigned long col) {
	 m->firstRow = addRow(m->firstRow, row, col); 

}


int getCol (tCol * first, unsigned long col) {
	if (first == NULL) 
		return 0; 
	if (first->col == col) 
		return 1; 
	return getCol(first->tail, col);  
}

int getRow (tRow * first, unsigned long row, unsigned long col) { 
	if (first == NULL) 
		return 0; 
	if (first->row == row) {
		return getCol(first->firstCol, col); 
	return getRow(first->tail, row, col); 
}

int get(const boolMatrixADT m, unsigned long row, unsigned long col) { 
	// si existe entonces es porque está en false, sino está en true 
	// recorro row y recorro col mientras row y col sean menor 
	return getRow(m->firstRow, row, col); 
}


