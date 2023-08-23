

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
  if(bitset__get(defended->triedCs,c)){
        return YES;
  }else{
     return NO;}
}

void adm__excludeC(struct TriedArguments* defended, int c){
  //printf("Now setting C: %d to tried\n", c);
  bitset__set(defended->triedCs,c);
  return;
}

void adm__set_triedB_if_necessary(struct TriedArguments* tried, struct TempExclude* tempExcl, struct AAF* aaf, int b){
  GSList* bAttackers = aaf->parents[b];
  bool allCsTriedForB = true;
  bool allTempCsExcludedForTempB = true;
  for (GSList *current = bAttackers; current != NULL; current = current->next){
    int currentI = *((int*) current->data);
    if(!adm__alreadyTriedC(tried, currentI)){
      allCsTriedForB = false;
      //printf("we have not yet tried C: %d\n", currentI);
      //break;
    }
    if(!adm__tempExcludeC_get(tempExcl, currentI)){
      allTempCsExcludedForTempB = false;
    }
  }
  if(allCsTriedForB){
    adm__triedB_set(tried, b);
    // all tried Bs 
    adm__excludeC(tried, b);
  }
  if(allTempCsExcludedForTempB){
    adm__tempExcludeB_set(tempExcl, b);
    adm__tempExcludeC_set(tempExcl, b);
  }
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

char* adm__triedC_print(struct TriedArguments* tried, struct AAF* aaf){
  int len = 100;
  char* str = (char*) malloc(len);
  int sidx = 0;
  str[sidx++] = '[';
  int isFirst = 1;
  for(int idx = 0; idx < aaf->number_of_arguments; idx++){
    if(sidx + strlen(aaf->ids2arguments[idx]) + 6 > len){
      len += 100;
      str = (char*) realloc(str, len);
    }
    if(isFirst != 0){
      strcpy(&str[sidx],aaf->ids2arguments[idx]);
      sidx += strlen(aaf->ids2arguments[idx]);
      str[sidx++] = '=';
      if(adm__alreadyTriedC(tried,idx) == YES)
        str[sidx++] = 'Y';
      else if(adm__alreadyTriedC(tried,idx) == NO)
        str[sidx++] = 'N';
      else
        str[sidx++] = 'X';
      isFirst = 0;
    } else{
      str[sidx++] = ',';
      strcpy(&str[sidx],aaf->ids2arguments[idx]);
      sidx += strlen(aaf->ids2arguments[idx]);
      str[sidx++] = '=';
      if(adm__alreadyTriedC(tried,idx) == YES)
        str[sidx++] = 'Y';
      else if(adm__alreadyTriedC(tried,idx) == NO)
        str[sidx++] = 'N';
      else
        str[sidx++] = 'X';
    }
  }
  str[sidx++] = ']';
  str[sidx] = '\0';
  return str;
}

char* adm__triedB_print(struct TriedArguments* tried, struct AAF* aaf){
  int len = 100;
  char* str = (char*) malloc(len);
  int sidx = 0;
  str[sidx++] = '[';
  int isFirst = 1;
  for(int idx = 0; idx < aaf->number_of_arguments; idx++){
    if(sidx + strlen(aaf->ids2arguments[idx]) + 6 > len){
      len += 100;
      str = (char*) realloc(str, len);
    }
    if(isFirst != 0){
      strcpy(&str[sidx],aaf->ids2arguments[idx]);
      sidx += strlen(aaf->ids2arguments[idx]);
      str[sidx++] = '=';
      if(adm__triedB_get(tried,idx) == YES)
        str[sidx++] = 'Y';
      else if(adm__triedB_get(tried,idx) == NO)
        str[sidx++] = 'N';
      else
        str[sidx++] = 'X';
      isFirst = 0;
    } else{
      str[sidx++] = ',';
      strcpy(&str[sidx],aaf->ids2arguments[idx]);
      sidx += strlen(aaf->ids2arguments[idx]);
      str[sidx++] = '=';
      if(adm__triedB_get(tried,idx) == YES)
        str[sidx++] = 'Y';
      else if(adm__triedB_get(tried,idx) == NO)
        str[sidx++] = 'N';
      else
        str[sidx++] = 'X';
    }
  }
  str[sidx++] = ']';
  str[sidx] = '\0';
  return str;
}

void adm__tried_destroy(struct TriedArguments* defended){
  bitset__destroy(defended->triedBs);
  bitset__destroy(defended->triedCs);
  free(defended);
}