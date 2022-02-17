#include "philo.h"

////parising---------------------->
int checkav(char *av)
{
	int     i;
	
	i = 0;
	while(av[i])
	{
		if(av[i] < '0' || av[i] > '9')
			return (0);
		i++;
	}
	return(1);
}

void argsinit(t_data *data, int ac, char **argv)
{
	data->nbphilo = atoi(argv[1]);
	data->t_die = atoi(argv[2]);
	data->t_eat = atoi(argv[3]);
	data->t_sleep = atoi(argv[4]);
	data->m_eat = -1;
	if(ac == 6)
		data->m_eat = atoi(argv[5]);
}

int getdata(int argc, char **argv, t_data *data)
{
	int i;

	i = 1;
	if(argc != 5 && argc != 6)
	{
		write(1,"wrong arguments!\n",strlen("wrong arguments!\n"));
		exit(1);
	}
	while(i < argc)
	{
		printf("\n%d\n",i);
		if(checkav(argv[i]) == 0)
		{	
			return (write(2, "invalid arguments!\n", 18));
		}
		i++;
	}
	argsinit(data, argc, argv);
	if ((data->m_eat < 0 && argc == 6) || data->nbphilo < 0
		|| data->t_die < 0 || data->t_eat < 0
		|| data->t_sleep < 0)
		return (write(2, "invalid arguments\n", 18));
	return(0);
}
//------------------------endof pr>
pthread_mutex_t *forks(t_data data)
{
	pthread_mutex_t  *mutex;
	int i;

	i = 0;
	mutex = malloc(sizeof(pthread_mutex_t)* data.nbphilo);
	while(i < data.nbphilo)
	{
		pthread_mutex_init(mutex + i, NULL);
		i++;
	}
	return (mutex);
}

t_philos *addphilos(t_data data, pthread_mutex_t *forks)
{
	t_philos *philos;
	pthread_mutex_t *dead;
	int	i;
	long l;

	pthread_mutex_init(&dead, NULL);
	philos = malloc(sizeof(t_philos) * data.nbphilo);
	philos->trds = malloc(sizeof(pthread_t) * data.nbphilo);
	t = get_time_stamp() / 1000;
	i = 0;
	while(i < data.nbphilo)
	{
		philos[i].id = i;
		philos[i].data = &data;
		philos[i].mutex = mutex;
		philos[i].t_meals = 0;
		philos[i].last_meal = t;
		philos[i].dead = &dead;
		if(pthread_create(philos->threads + i,NULL, &dowhatphilodo ,philos[i]) != 0);
			write(2, "Faild to creat thread\n", 22);
		usleep(100);
		i++;
	}
	return(philos);
}

int main(int argc,char **argv)
{
	t_data  data;
	pthread_mutex_t forks;
	t_philos 	*philos;

	if(getdata(argc,argv,&data) != 0)
		return (1);
	forks = forks(data);
	return(0);
	philos = addphilos(data,forks);
	//need supervisor and philos routine
}

//-------->
long	get_time_stamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (long)1000000 + tv.tv_usec);
}
