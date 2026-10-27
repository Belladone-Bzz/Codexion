# include "codexion.h"

typedef struct s_test_data
{
	pthread_mutex_t	*locker;
	int				*counter;
	char			*name;
}	t_test_data;

void	*thread_function_test(void *data)
{
	t_test_data		*thread_data;
	int				index;

	thread_data = (t_test_data *)data;
	index = 0;
	while (index < 100000)
	{
		pthread_mutex_lock(thread_data->locker);
		printf("%s - Compteur: %d\n", thread_data->name, *thread_data->counter);
		(*thread_data->counter) ++;
		pthread_mutex_unlock(thread_data->locker);
		index ++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t			test1;
	pthread_t			test2;
	pthread_mutex_t		locker;
	int					counter;
	t_test_data			data1;
	t_test_data			data2;

	counter = 0;
	if (pthread_mutex_init(&locker, NULL) != 0)
		return(1);
	data1.locker = &locker;
	data1.counter = &counter;
	data1.name = "Thread 1";
	data2.locker = &locker;
	data2.counter = &counter;
	data2.name = "Tread 2";
	if (pthread_create(&test1, NULL, thread_function_test, &data1) != 0)
	{
		printf("Erreur lors de la creation du thread 1.\n");
		return (1);
	}
	if (pthread_create(&test2, NULL, thread_function_test, &data2) != 0)
	{
		printf("Erreur lors de la creation du thread 2.\n");
		return (1);
	}
	pthread_join(test1, NULL);
	pthread_join(test2, NULL);
	pthread_mutex_destroy(&locker);
	return (0);
}
