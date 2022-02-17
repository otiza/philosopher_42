#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_data
{
    int nbphilo;
    int t_die;
    int t_eat;
    int t_sleep;
    int m_eat;
}   t_data;

typedef struct s_philo
{
    int     id;
    int     last_meal;
    int     t_meals;
    t_data  *data;
    pthread_mutex_t *mut;
    pthread_mutex_t *dead;
    pthread_t *trds;
}   t_philos;

int getdata(int argc, char **argv, t_data *data);
void argsinit(t_data *data, int ac, char **argv);
int checkav(char *av);
#endif