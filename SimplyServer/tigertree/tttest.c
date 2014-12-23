#include "tigertree.h"
#include <string.h>

void tttest(char *s, int repeat) 
{
  TT_CONTEXT ctx;
  unsigned char hash[TIGERSIZE];
  int i;

  printf("TigerTree hash of \"%s\"",s);
  if(repeat>1) {
    printf(", repeated %d times",repeat);
  }
  printf(":\n");

  tt_init(&ctx);

  for (i = 0; i<repeat; i++) {
    tt_update(&ctx,s,strlen(s));
  }
  tt_digest(&ctx,hash);

  printf("  ");
  for (i = 0; i<TIGERSIZE; i++) {
    printf("%.2x", hash[i]);
  }
  printf("\n");
}

main()
{
  tttest("",1);
  tttest("1",1);
  tttest("abc",1);
  tttest("Tiger",1);
  tttest("TigerTree",1);
  tttest("a",1024);
  tttest("a",1025);
  tttest("b",17408);
  tttest("b",17409);
}

