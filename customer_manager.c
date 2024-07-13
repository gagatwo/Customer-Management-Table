#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"
#define _GNU_SOURCE 1
#define HASH_MULTIPLIER 65599
#define BUCKET_COUNT 1024

    

static int hash_function(const char *pcKey, int iBucketCount)//define hash_function
{
    int i;
    unsigned int uiHash = 0U;
    for (i = 0; pcKey[i] != '\0'; i++)
        uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
            + (unsigned int)pcKey[i];
    return (int)(uiHash % (unsigned int)iBucketCount);
}

struct userInfo//define userinfo
{
    char* name;
    char* id;
    int asset;
    struct userInfo* next_name;
    struct userInfo* next_id; 
};

struct Table//dfine table
{
    struct userInfo *array[BUCKET_COUNT];
};

struct DB//define DB
{
  struct Table *t_name;
  struct Table *t_id;   
};


DB_T CreateCustomerDB(void)
{
    
    DB_T d;

    d = (DB_T) calloc(1, sizeof(struct DB));//define d
    if (d == NULL) {
        fprintf(stderr, "Can't allocate a memory for DB_T\n");
        return NULL;  
    }
    d->t_name = (struct Table *)calloc(1, sizeof(struct Table));//definte hashtable t_name
    d->t_id = (struct Table *)calloc(1, sizeof(struct Table));//definte hashtable t_id
    
    if (d->t_name == NULL || d->t_id == NULL) {
        fprintf(stderr, "Can't allocate a memory for table\n");   
        free(d);
        return NULL;
    }
    return d;
  
    return NULL;
}


/*--------------------------------------------------------------------*/
//Destroy the Customer DataBase
void DestroyCustomerDB(DB_T d)
{
  //check the initial value
  assert(d!=NULL);

  struct userInfo *p;
  struct userInfo *nextp;
  
  //free every node
  for(int i=0; i<BUCKET_COUNT; i++)
  {
    for(p = d->t_name->array[i] ; p != NULL; p = nextp)
    {
      nextp=p->next_name;
      free(p->name);
      free(p->id);
      free(p);
    }
  }

  //free hash tables and DB
  free(d->t_name);
  free(d->t_id);
  free(d);
}


/*--------------------------------------------------------------------*/
//Register the customer information on DataBase
int RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int asset)
{
  //check the initial value
  if(d==NULL||id==NULL||name==NULL) return -1;
  else if(asset <= 0) return -1;

  struct userInfo* q;
  int hname = hash_function(name, BUCKET_COUNT);
  int hid = hash_function(id, BUCKET_COUNT);

  //check the overlap node
  for(q=d->t_name->array[hname] ; q!=NULL ; q=q->next_name)
      if(strcmp(q->name, name)==0)
          return -1;

  for(q=d->t_id->array[hid] ; q!=NULL ; q=q->next_id)
      if(strcmp(q->id, id)==0)
          return -1;

  //add the node and register the value
  struct userInfo* p = (struct userInfo*)malloc(sizeof(struct userInfo));
  p->name = strdup(name);
  p->id = strdup(id);
  p->asset = asset;
  p->next_id = d->t_id->array[hid];
  p->next_name = d->t_name->array[hname];
  d->t_id->array[hid] = p;
  d->t_name->array[hname] = p;
  return 0;
}
/*--------------------------------------------------------------------*/
//Unregister the customer information from DB using the customer's ID
int UnregisterCustomerByID(DB_T d, const char *id)
{
  //check the initial value
  if(d == NULL || id == NULL) return -1;

  struct userInfo* p;
  struct userInfo* prevp = NULL;
  char *namekey;
  int find = 0;
  int hid = hash_function(id, BUCKET_COUNT);

  //find the node that we must delete, and connect the previous id_node and next id_node
  for(p=d->t_id->array[hid] ; p!=NULL ; p=p->next_id)
  {
    if(strcmp(p->id, id)==0)
    {
      namekey = p->name;
      if(prevp == NULL)
        d->t_id->array[hid] = p->next_id;
      else
        prevp->next_id = p->next_id;

      find = 1;
      break;
    }
    prevp = p;
  }

  //if find the node that we must delete, execute this statement
  if(find)
  {
    //connect previous name_node and next name_node
    prevp = NULL;
    int hname = hash_function(namekey, BUCKET_COUNT);
    for(p=d->t_name->array[hname] ; p!=NULL ; p=p->next_name)
    {
      if(strcmp(p->name, namekey)==0)
      {
        if(prevp == NULL)
          d->t_name->array[hname] = p->next_name;
        else
          prevp->next_name = p->next_name;

        free(p->name);
        free(p->id);
        free(p);
        return 0;
      }
      prevp = p;
    }
  }

  return -1;
}

/*--------------------------------------------------------------------*/
//Unregister the customer information from DB using the customer's Name
int UnregisterCustomerByName(DB_T d, const char *name)
{
  //check the initial value
  if(d == NULL || name == NULL) return -1;

  struct userInfo* p;
  struct userInfo* prevp = NULL;
  char *idkey;
  int find = 0;
  int hname = hash_function(name, BUCKET_COUNT);

  //find the node that we must delete, and connect the previous name_node and next name_node
  for(p=d->t_name->array[hname] ; p!=NULL ; p=p->next_name)
  {
    if(strcmp(p->name, name)==0)
    {
      idkey = p->id;
      if(prevp == NULL)
        d->t_name->array[hname] = p->next_name;
      else
        prevp->next_name = p->next_name;
      
      find = 1;
      break;
    }
    prevp = p;
  }

  //if find the node that we must delete, execute this statement
  if(find)
  {
    //connect previous name_node and next name_node
    prevp = NULL;
    int hid = hash_function(idkey, BUCKET_COUNT);
    for(p=d->t_id->array[hid] ; p!=NULL ; p=p->next_id)
    {
      if(strcmp(p->id, idkey)==0)
      {
        if(prevp == NULL)
          d->t_id->array[hid] = p->next_id;
        else
          prevp->next_id = p->next_id;
        
        free(p->name);
        free(p->id);
        free(p);
        return 0;
      }
      prevp = p;
    }
  }

  return -1;
}
/*--------------------------------------------------------------------*/
//Find the customer's asset from DB using his ID
int GetAssetByID(DB_T d, const char* id)
{
  //check the initial value
  if(d == NULL || id == NULL) return -1;

  //find the node and get asset
  struct userInfo* p;
  int hid = hash_function(id, BUCKET_COUNT);
  for(p=d->t_id->array[hid] ; p!=NULL ; p=p->next_id)
    if(strcmp(p->id, id)==0)
        return p->asset;    

   return -1;
}
/*--------------------------------------------------------------------*/
//Find the customer's asset from DB using his Name
int GetAssetByName(DB_T d, const char* name)
{
  //check the initial value
  if(d == NULL || name == NULL) return -1;

  //find the node and get asset
  struct userInfo* p;
  int hname = hash_function(name, BUCKET_COUNT);
  for(p=d->t_name->array[hname] ; p!=NULL ; p=p->next_name)
    if(strcmp(p->name, name)==0)
      return p->asset;    

  return(-1);
}

