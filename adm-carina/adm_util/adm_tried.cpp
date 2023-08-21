#define NO 0
#define YES 1


struct TriedArguments{
  struct BitSet *triedBs;
  struct BitSet *triedCs;
};


void adm__tried_init(struct TriedArguments* defended){
  defended->triedBs = (struct BitSet*) malloc(sizeof(struct BitSet));
  defended->triedCs = (struct BitSet*) malloc(sizeof(struct BitSet));
}

/**
 * Returns the label of the given argument.
 */
int adm__triedB_get(struct TriedArguments* defended, int arg){
    if(bitset__get(defended->triedBs,arg))
        return YES;
    else return NO;
}


void adm__triedB_set(struct TriedArguments* defended, int b){
  bitset__set(defended->triedBs,b);
  return;
}


bool adm__alreadyTriedC(struct TriedArguments* defended, int c){
  if(bitset__get(defended->triedCs,c))
        return YES;
    else return NO;
}

void adm__excludeC(struct TriedArguments* defended, int c){
  bitset__set(defended->triedCs,c);
  return;
}

void adm__triedC_set(struct TriedArguments* defended, struct AAF* aaf, int b, int c){
  bitset__set(defended->triedCs,c);
  GSList* bAttackers = aaf->parents[b];
  bool allCsTriedForB = true;
  for (GSList *current = bAttackers; current != NULL; current = current->next){
    int currentI = *((int*) current->data);
    if(!adm__alreadyTriedC(defended, currentI)){
      allCsTriedForB = false;
      break;
    }
  }
  if(allCsTriedForB){
    adm__triedB_set(defended, b);
  }
  return;
}

void adm__tried_destroy(struct TriedArguments* defended){
  bitset__destroy(defended->triedBs);
  bitset__destroy(defended->triedCs);
  free(defended);
}