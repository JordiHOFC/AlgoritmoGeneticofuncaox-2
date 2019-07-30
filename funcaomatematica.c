 #include<stdio.h>
 #include<stdlib.h>
 #include<math.h>
 
 typedef struct Sindividuo{
     int * genotipo;
     float fitness;
     int probabilidadedecruzamento;
 }individuo;

 int l=16;
 int primeirodointervalo=0;
 int ultimodointervalo=5;

 individuo * criarIndividuo(individuo * novo){
    int i;
     novo->genotipo=(int*)malloc(l*sizeof(int));
    for(i=0;i<l;i++){
        novo->genotipo[i]=rand() %2;
    }
   
 }
 void print(int *v){
    int i;
    for(i=0;i<l;i++){
        printf("%d\t",v[i]);
    }
 }

 int decodificador(int *bin){
	int dec=0;
	int i=15;
	int p=0;
	while(i>=0){
		dec=dec+pow(2,i)*(bin[p]%10) *bin[p];
		p++;
		i--;
	}
	return dec;
}

float precisao(){
    int primeirodointervalo=0;
    int ultimodointervalo=5;
    float x=(ultimodointervalo-primeirodointervalo)/(pow(2,l))-1;
  
    return x;
}
void Fitness(individuo * novo, float precisao,int primeirodointervalo){
    novo->fitness=primeirodointervalo+decodificador(novo->genotipo)*precisao;
    novo->fitness=pow(novo->fitness,2);
}
 individuo * criarPopulacao(int tamanho){
     int i,indice;
     float x=precisao();
     float maior=0;
     FILE * fp=fopen("melhoresindividuos.txt","w+");
     individuo *Populacao =(individuo*)malloc(tamanho*sizeof(individuo));
     for(i=0;i<tamanho;i++){
         criarIndividuo(&Populacao[i]);
         Fitness(&Populacao[i],x,primeirodointervalo);
         //agora vamos encontrar qual individuo tem a melhor aptidao
        if(maior<Populacao[i].fitness){
            maior=Populacao[i].fitness;
            indice=i;
        }
     }
     fprintf(fp,"individuo %d  fitness: %f POPULACAO INICIAL",indice,maior);
     fclose(fp);
    return Populacao;
 }
 /*aqui vamos fazer um cruzamento genetico, definido por um ponto de corte: e fazer 2 filhos com metades dos dois pais que sao divididos pelo indices
  pai1 = 1,2,3 ; pai2 4,5,6; ponto de corte=2;
  filho1 = 1,2,6; filho2 4,5,3
 */
 void crossover(individuo * pai1,individuo * pai2,individuo *f1,individuo *f2){
     int pontodecorte,i,n;
    //agora vamos sortear o ponto e fazer os novos filhos
    pontodecorte=rand() % 14;
   // printf("ponto de corte %d",pontodecorte);
    if(pontodecorte ==0 || pontodecorte==15){
        pontodecorte=rand() % 14;
    }
   // print(pai1->genotipo);
    //print(pai2->genotipo);
    //agora vamos criar os novos filhos
    for(i=0;i<pontodecorte;i++){
        (*f1).genotipo[i]=(*pai1).genotipo[i];;
        (*f2).genotipo[i]=(*pai2).genotipo[i];
    }
    for(i=pontodecorte;i<16;i++){
        f1->genotipo[i]=pai2->genotipo[i];
        f2->genotipo[i]=pai1->genotipo[i];
    }
 }
 //nosso mecanismo de mutação é o bitflip: onde selecionamos um indice de forma aleatoria e trocamos seu valor, caso ele seja 0 se torna 1 e vice e versa
void mutacao(individuo * novo){
    int moeda,pm=5,indice;
    //primeiro vamos jogar uma moeda para testar se o individuo vai ou n sofrer mutacao, se o valor da moeda for menor ou igual a pm ele sera mutado
    moeda=rand() %100;
    if(moeda<=pm){
        indice=rand() % 15;
       // printf("\nindice %d",indice);
        if(novo->genotipo[indice]==1){
            novo->genotipo[indice]=0;
        }else{
           novo->genotipo[indice]=1; 
        }
    } 


}
individuo * selecaoTorneio(individuo* populacao){

    int K=80;
    int r;
    int individuo1=rand()%20;
    int individuo2=rand()%20;
    
   
    float S;
    individuo * best,*pior;
    
    if(populacao[individuo1].fitness>populacao[individuo2].fitness){
        best=&populacao[individuo1];
        pior=&populacao[individuo2];
    }else{
        best=&populacao[individuo2];
        pior=&populacao[individuo1];
    }
    r=rand()%100;
    if(r<K){
        return best;
    }else{
        return pior;
    }
   

}
void Avaliacao(individuo *Populacao,int geracao){
    int i,indice;
    float x=precisao(),maior=0;
    FILE * fp=fopen("melhoresindividuos.txt","a+");
    for(i=0;i<20;i++){
         Fitness(&Populacao[i],x,primeirodointervalo);
         //agora vamos encontrar qual individuo tem a melhor aptidao
        if(maior<Populacao[i].fitness){
            maior=Populacao[i].fitness;
            indice=i;
        }
     }
     fprintf(fp,"\nindividuo %d  fitness: %f geracao :%d",indice,maior,geracao);
     fclose(fp);

}

 int main(){
        individuo *pai1=(individuo*)malloc(sizeof(individuo)),*pai2=(individuo*)malloc(sizeof(individuo)),*f1=(individuo*)malloc(sizeof(individuo)),*f2=(individuo*)malloc(sizeof(individuo));
        criarIndividuo(f1);
        criarIndividuo(f2);
        criarIndividuo(pai1);
        criarIndividuo(pai2);
        
        individuo *po=criarPopulacao(20);
        individuo *novapopulacao=(individuo*)malloc(20*sizeof(individuo));
        int i,cont,j;
        
        for(j=0;j<200;j++){
            printf("\n geracao :%d\n",j);
            //selecao, cruzamento e mutacao
            for(i=0;i<10;i++){
                pai1=selecaoTorneio(po);
                pai2=selecaoTorneio(po);
                crossover(pai1,pai2,f1,f2);
                mutacao(f1);
                mutacao(f2);
               
                if(i==0){
                    novapopulacao[i]=(*f1);
                    novapopulacao[i+1]=(*f2);
                    cont=i+1;
                    
                }
                novapopulacao[cont+1]=(*f1);
                novapopulacao[cont+2]=(*f2);
                cont=cont+2;

            }
            //avaliacao
            Avaliacao(novapopulacao,j);
            //passsamos a nova populacao para a populacao ofical
            for(i=0;i<20;i++){
                po[i]=novapopulacao[i];
            }
        } 
   
     
    return 0;
 }