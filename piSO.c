#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

void funcionSomm(int);
void funcionMozo(int);
void funcionJefe(int);

int main(int argc, char * argv[]) {
    
    printf("\n\n ASI ES UN DIA NORMAL EN EL RESTAURANTE SAL MARINA, EN SANTA EULALIA DEL RIO, IBIZA\n\n");
    //###############################
    //CREACION E INICIALIZACION DE VARIABLES
    //###############################
    int pid;
    int elementos = /*((int)argv[1])+1*/7;
    int *pid_h = malloc(sizeof(int) * elementos);
    srand(time(NULL));
    struct sigaction funcionSommss, funcionJefess;
    pid_t hpid_chef;
    int statusChef;

    //###############################
    //FOR QUE CREA LOS HIJOS
    //###############################
    for(int i=0; i<elementos; i++){
        pid = fork();
        srand(time(NULL)+i);
        if(pid==-1){ //CUANDO DETECTAMOS ALGUN ERROR
          perror("ERROR CREANDO ALGUNO DE LOS PROCESOS HIJOS\n");

        }else if(pid==0){ //CODIGO PARA TODOS LOS HIJOS
          switch(i){
              case 0: //SOMMELIER
                  //kill(getpid(), SIGSTOP);
                  funcionSommss.sa_handler=funcionSomm;
                  //printf("Sommelier despierto\n");
                  //EL CHEF ENVIA QUE NO HAY VINO
	                if(-1==sigaction(SIGUSR1, &funcionSommss, NULL)){
                      perror("Llamada a signal.\n");
                      exit(-1);
                	}
                  //EL CHEF ENVIA QUE NO HAY INGREDIENTES
                  if(-1==sigaction(SIGUSR2, &funcionSommss, NULL)){
                      perror("Llamada a signal.\n");
                      exit(-1);
                  }
                  pause();
                  //ESPERAMOS QUE SE HAGA TODO EL PROCESO Y MATAMOS AL SOMMELIER, O BIEN DESDE EL MISMO O DESDE EL CHEF
                  sleep(10);
                  kill(getpid(), SIGKILL);
                  break;
              case 1:
                  //kill(getpid(), SIGSTOP);
                  funcionJefess.sa_handler=funcionJefe;
                  //printf("Jefe de sala despierto\n");
                  //EL CHEF ENVIA QUE NO HAY VINO
                  if(-1==sigaction(SIGUSR1, &funcionJefess, NULL)){
                      perror("Llamada a signal.\n");
                      exit(-1);
                  }
                  pause();
                  sleep(10);
                  kill(getpid(), SIGKILL); //Por si acaso el proceso no esta muerto, lo mato
                  break;
              default: //PINCHES DE COCINA
                  kill(getpid(), SIGSTOP);
                  //pause();
                  //printf("Pinche %d despierto\n", i-1);
                  printf("\t- El pinche %d se pone a trabajar en un plato\n", i-1);
                  int temp = (rand()%3)+3;
                  sleep(temp);
                  int res = rand()%2;
                  //printf("El pinche %d ha devuelto un: %d\n", i-1, res);
                  if(res == 1){
                      printf("\t- El pinche %d ha completado el plato\n", i-1);
                  }else{
                      printf("\t- El pinche %d no ha completado el plato\n", i-1);
                  }
                  exit(res);
                  kill(getpid(), SIGKILL); //Por si acaso el proceso no esta muerto, lo mato
          }
          break;

        }else{ //CODIGO PARA EL PADRE, EL CHEF
            *(pid_h+i) = pid;
        }
    }

    if(pid!=0){ //CODIGO PRINCIPAL DEL CHEF

        printf("- El chef entra en el restaurante para iniciar una jornada de trabajo\n");
        //printf("Durmiendo los 3 segundos...\n\n");
        printf("- El chef empieza a organizar la cocina\n");
        sleep(3); //El chef espera 3 segundos en los que se dedica a organizar la cocina
        srand(time(NULL)+rand()%50);
        int aleat1 = rand()%2;
        //kill(*pid_h, SIGCONT);
        //sleep(2);
        if(aleat1==0){
            //FALTAN INGREDIENTES
            //printf("No hay ingredientes\n");
            printf("- El chef acaba de organizar la cocina, pero se da cuenta de que faltan ingredientes\n");
            kill(* (pid_h+0), SIGUSR1); //SIGUSR1 - Despertamos al sommelier
            printf("- El chef manda al sommelier a por ingredientes\n");
        }else{
            //FALTA VINO
            printf("- El chef acaba de organizar la cocina, pero se da cuenta de que falta vino\n");
            //printf("No hay vino\n");
            kill(* (pid_h+0), SIGUSR2); //SIGUSR2 - Despertamos al sommelier
            printf("- El chef manda al sommelier a por vino\n");
        }
        printf("- Mientras el chef espera la respuesta del sommelier\n");
        //kill(getpid(), SIGSTOP); //NOS PARAMOS PARA DEJAR ACTUAR AL SOMMELIER
        //pause();
        //sleep(10);
        //pause();
        //printf("Esperando para leer\n");
        hpid_chef = wait(&statusChef);
        int salida = WEXITSTATUS(statusChef); //LEEMOS LO QUE NOS DICE EL SOMMELIER
        kill(* (pid_h), SIGKILL); //MATAMOS AL SOMMELIER
        //printf("El sommelier devuelve: %d\n", salida); //IMPRIMO LO QUE ME LLEGA DEL SOMMELIER

        //#########################################
        //en funcion de lo que nos diga el somm, vamos a:
        //#########################################

        switch(salida){
            case 1:
                  //printf("ESTO ES INDIGNANTE! NO HAY VINO\n TODOS DESPEDIDOS\n CIERRO\n");
                  printf("- El chef considera que la falta de vino es un problema grande\n- Decide echar a todos del restaurante y cerrarlo\n");
                  for(int i=0; i<elementos; i++){
                      kill(*(pid_h+i), SIGKILL);
                  }
                  printf("\n \x1b[31m -- RESTAURANTE CERRADO --  \n");
                  //printf("Todos los procesos muertos, fin programa");
                  //FIN programa
                  return 0;
                  //FIN programa
                  break;
            case 2:
                  //printf("Chicos, faltan algunos ingredientes - dice el chef\n");
            printf("- El chef sabe que faltan algunos ingredientes, pero eso no le detiene\n");
            default:
                  printf("- El chef pone a trabajar a todos los pinches en la cocina\n");
                  //printf("TODOSLOS PINCHES A TRABAJAR!!\n");
        }

        //#########################################
        //hay que poner a los pinches a trabajar
        //#########################################

        for(int i=2; i<elementos; i++){
          kill(*(pid_h+i), SIGCONT);
        }
        int si = 0, no=0;
        for(int i=0; i<elementos-2;i++){
          hpid_chef = wait(&statusChef);
          salida = WEXITSTATUS(statusChef); //LEEMOS A LOS PINCHES
          if(salida==1){
              si++;
          }else{
              no++;
          }
        }

        //###############################################
        //actuamos en consecuencia con el trabajo de los pinches
        //###############################################

        if(si==0){
          printf("- El chef ve que no hay ningún plato preparado\n- Se decepciona con el trabajo de sus pinches\n");
          printf("- Decide echar a todos del restaurante y cerrarlo\n");
          printf("\n \x1b[31m -- RESTAURANTE CERRADO --  \n");
          //printf("ESTO ES INDIGNANTE! NINGUN PLATO BIEN HECHO\n TODOS DESPEDIDOS\n CIERRO\n");
          for(int i=0; i<elementos; i++){
               kill(*(pid_h+i), SIGKILL);
          }
          //printf("Todos los procesos muertos, fin programa");
          //FIN programa
          return 0;
          //FIN programa
        }else if(no==0){
          printf("- El chef está orgulloso del trabajo de sus pinches de cocina\n- Le dará un aumento a todos ellos por el trabajo realizado\n");
          //printf("Vale, JEFE DE SALA!, TE TOCA\n");
        }else{
          printf("- El chef observa el trabajo de sus pinches, y aunque no le disgusta, no es lo que más gracia le hace\n");
        }
        printf("- El chef manda al jefe de sala a colocar las mesas\n");

        //###################################################
        //AHORA VAMOS A LLAMAR AL JEFE DE SALA PARA QUE ACTUE
        //###################################################

        kill(*(pid_h+1), SIGCONT);
        sleep(1);
        kill(*(pid_h+1), SIGUSR1); //SIGUSR1 - Despertamos al jefe de sala
        //printf("Esperando a que el jefe organice\n");
        hpid_chef = wait(&statusChef);
        salida = WEXITSTATUS(statusChef); //LEEMOS LO QUE NOS DICE EL JEFE DE SALA
        if(salida == 1)
            kill(*(pid_h+1), SIGKILL); //MATAMOS AL JEFE DE SALA
        printf("- Cuando todo está listo, el chef procede a abrir el restaurante al público\n");
        //printf("El jefe de sala ya ha terminado\n");
        printf("\n \x1b[34m  -- RESTAURANTE ABIERTO --  \n");
        //printf("\nABRIMOS EL RESTAURANTE\n\n");

    }
  return 0;
}

void funcionSomm(int sig){
  //printf("Funcion manejadora del sommelier\n"); //DEBUGGER PARA SABER QUE LLEGO AQUI

  struct sigaction funcionMozoss;
  pid_t hpid_som;
  int statusSom;
  int pid_mozo = fork();

  if(pid_mozo == -1){
    perror("Ha habido un error al crear al mozo\n");
  }else if(pid_mozo == 0){
    funcionMozoss.sa_handler=funcionMozo;
               
    if(-1==sigaction(SIGPIPE, &funcionMozoss, NULL)){
       perror("Llamada a signal.\n");
       exit(-1);
    }
    
    pause();
    
    //ESPERAMOS QUE SE HAGA TODO EL PROCESO Y MATAMOS AL MOZO, O BIEN DESDE EL MISMO O DESDE EL SOMMELIER
    sleep(10);
    kill(getpid(), SIGKILL);
  }else{
    sleep(5);//LE DAMOS TIEMPO A DESPEREZARSE
    printf("\t- El sommelier llama al mozo para que vaya a buscarlo\n");
    
    sleep(2);
    kill(pid_mozo, SIGPIPE); //LE MANDAMOS A POR ALGO
    
    printf("\t- El sommelier espera la respuesta del mozo\n"); //LEEMOS LO QUE NOS MANDA EL MOZO
    
    hpid_som = wait(&statusSom);
    int resultado = WEXITSTATUS(statusSom); //LEEMOS LO QUE NOS DICE EL SOMMELIER
    
    if(sig == 10){ //si faltaban ingredientes
      if(resultado == 0){
          printf("\t- El sommelier informa al chef de que no ha podido localizar los ingredientes que faltaban\n");
          exit(2);
      }else{
          printf("\t- El sommelier informa al chef de que ha encontrado los ingredientes que faltaban\n");
          exit(3);
      }
    }else{ //si faltaba vino
      if(resultado == 0){
          printf("\t- El sommelier informa al chef de que no ha podido localizar el vino que faltaba\n");
          exit(1);
      }else{
          printf("\t- El sommelier informa al chef de que ha encontrado el vino que faltaba\n");
          exit(3);
      }
    }
  }
}

void funcionMozo(int sig){
  printf("\t\t- El mozo va a buscarlo\n");
  sleep(1);
  
  srand(time(NULL));
  int num = rand()%2;
  if(num==1){
      printf("\t\t- El mozo ha encotrado lo que estaba buscando\n");
  }else{
      printf("\t\t- El mozo no ha encotrado lo que estaba buscando\n");
  }
  
  exit(num); //LE MANDO UN NUM ALEATORIO ENTRE 0 Y 1
}

void funcionJefe(int sig){
 
  printf("\t- El jefe de sala empieza a colocar la sala del restaurante\n");
  sleep(3);
  printf("\t- El jefe de ha acabado de colocar sala del restaurante\n");
  exit(1);
}