#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define N 300.0 //Tamanho da Malha
#define dom 14.0
#define IT 300 //Loops temporais
double ** aloc_matrix(){
	//Alocação de memoria para a matriz
	int i;
	double **m;
	m = malloc(N*sizeof(double *));
	for(i = 0; i < N; i++)
		m[i]=malloc(N*sizeof(double *));
	return m;	
}

void zera_malha(double **m){
	int i,j;
	for(i = 0; i < N; i++)
		for( j = 0; j < N; j++)
			m[i][j] = 0.0;
}
void imprime_matriz(double **m,double dx,double dy,double dt){
	int i,j;
	double x,y;
	y = -7.0;
	for(j = 0; j < N; j++){
		x = -7.0;
		for(i = 0; i < N; i++){
			printf("%12.6lf\t%12.6lf\t%12.6lf\n",x,y,m[i][j]);
			x = x + dx; 
		}
		y = y + dy;
	}

}
void initial_cond(double **m, double dx, double dy, double dt){
	int i,j;
	double x,y;
	y = -7.0;
	for(j = 0; j < N; j++){
		x = -7.0;
		for( i = 0; i < N; i++){
			m[i][j] = 4 * atan( exp( 3 - sqrt( x*x + y*y ) ));
			x = x + dx;
		}
		y = y + dy;
	}	
}
int main(){
		
	double **m, **aux,**ant, dx,dy,dt,dxts,dts,x,y;
	int i,j,k,ite=0;
	FILE *pf;
	char arq[50];
	
	//Definição de Parâmetros
	dx = dom/N;
	dy = dom/N;
	dt = dx / sqrt(2.0);
	dts = dt*dt;
    dxts = (dt/dx)*(dt/dx);

	m = aloc_matrix();
	aux = aloc_matrix();
    ant = aloc_matrix();

	zera_malha(m);
	initial_cond(m,dx,dy,dt);
	
	//Aplicação das Discretizações de Diferenças Finitas
	while(ite < IT){ //Loop temporal
		// Abertura de arquivo para o tempo t
		sprintf(arq,"t-%d.dat",ite);
		pf = fopen(arq,"w");
		for(i = 1; i < N-1; i++){ //Loop Percorrendo a Coluna da malha
			for(j = 1; j < N-1; j++){ //Loop Percorrendo a Linha da malha	
				// Fazendo o loop apenas em partes inferiores ao contorno da malha podemos retirar o a atualização dos dados da condição de contorno por meio de
				// condicionais que gastam muito mais operações no processador
				aux[i][j] = -ant[i][j] + 2.0*( 1.0 - 2.0*dxts )*m[i][j] + dxts*( m[i+1][j] + m[i-1][j] + m[i][j+1] + m[i][j-1] ) - dts * ( sin( ( m[i+1][j] + m[i-1][j] + m[i][j+1] + m[i][j-1] )/4.0 ) );
			}
		}

        //Salvando do tempo anterior 
		for( i = 1; i < N-1; i++ ){
			for( j = 1; j < N-1; j++){
				ant[i][j] = m[i][j];
			}
		}            

		//atualização da matriz de solução 
		for( i = 1; i < N-1; i++ ){
			for( j = 1; j < N-1; j++){
				m[i][j] = aux[i][j];
			}
		}	

		//Escrita no arquivo
        	y = -7.0;
        	for(j = 0; j < N; j++){
        		x = -7.0;
        		for(i = 0; i < N; i++){
        			fprintf(pf,"%12.6lf\t%12.6lf\t%12.6lf\n",x,y,m[i][j]);
        			x = x + dx; 
        		}
        		y = y + dy;
        	}

		fclose(pf);
		ite++ ;
	}
	return 0;
}
