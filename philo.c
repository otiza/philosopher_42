#include "philo.h"
//---->utils
long	get_time_stamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (long)1000000 + tv.tv_usec);
}
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
//philos routinea

void	p_eat(t_philos *i)
{
	i->last_meal = get_time_stamp() / 1000;
	pthread_mutex_lock(i->dead);
	printf("%ld %d is eating\n", i->last_meal, i->id + 1);
	pthread_mutex_unlock(i->dead);
	ft_usleep(i->data->t_eat * 1000);
}

void	p_sleep(t_philos *i)
{
	pthread_mutex_lock(i->dead);
	printf("%ld %d is sleeping\n", get_time_stamp() / 1000, i->id + 1);
	pthread_mutex_unlock(i->dead);
	ft_usleep(i->data->t_sleep * 1000);
}

void	p_think(t_philos *i)
{
	pthread_mutex_lock(i->dead);
	printf("%ld %d is thinking\n", get_time_stamp() / 1000, i->id + 1);
	pthread_mutex_unlock(i->dead);
}

void	p_takefork(t_philos *i)
{
	pthread_mutex_lock(i->dead);
	printf("%ld %d has taken a fork\n", get_time_stamp() / 1000, i->id + 1);
	pthread_mutex_unlock(i->dead);
}

void	*dowhatphilodo(void *arg)
{
	t_philos	*main;

	main = (t_philos *)arg;
	while (1)
	{
		p_think(main);
		pthread_mutex_lock(&main->mut[main->id]);
		p_takefork(main);
		pthread_mutex_lock(&main->mut[(main->id + 1)
			% main->data->nbphilo]);
		p_takefork(main);
		p_eat(main);
		pthread_mutex_unlock(&main->mut[main->id]);
		pthread_mutex_unlock(&main->mut[(main->id + 1)
			% main->data->nbphilo]);
		if (main->data->m_eat != -1)
			main->t_meals++;
		if (main->data->m_eat == main->t_meals)
			break ;
		p_sleep(main);
	}
	return (0);
}

//void dowhatphilodo(t_philos **philos);


//void *takefork();

//------------------------endof pr>
pthread_mutex_t *forksinit(t_data data)
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
	l = get_time_stamp() / 1000;
	i = 0;
	while(i < data.nbphilo)
	{
		philos[i].id = i;
		philos[i].data = &data;
		philos[i].mut = forks;
		philos[i].t_meals = 0;
		philos[i].last_meal = l;
		philos[i].dead = &dead;
		if(pthread_create(philos->trds + i, NULL, &dowhatphilodo,&philos[i]) != 0);
			write(2, "Faild to creat thread\n", 22);
		usleep(100);
		i++;
	}
	return(philos);
}

int	supervisor(t_data args, t_philos *philos)
{
	int		finish_eat;
	int		i;
	long	t;

	finish_eat = 0;
	while (finish_eat != args.nbphilo)
	{
		finish_eat = 0;
		i = -1;
		while (++i < args.nbphilo)
		{
			t = get_time_stamp() / 1000;
			if (philos[i].t_meals == args.m_eat)
				finish_eat++;
			else if (t - philos[i].last_meal >= args.t_die)
			{
				pthread_mutex_lock(philos[i].dead);
				if (t - philos[i].last_meal >= args.t_die)
					return (printf("%ld %d Died\n", t, philos[i].id + 1));
				else
					pthread_mutex_unlock(philos[i].dead);
			}
		}
	}
}
int main(int argc,char **argv)
{
	t_data  data;
	pthread_mutex_t *forks;
	t_philos 	*philos;

	if(getdata(argc,argv,&data) != 0)
		return (1);
	forks = forksinit(data);
	philos = addphilos(data, forks);
	supervisor(data, philos);
	return(0);
	
	//need  philos routine
}

//-------->

