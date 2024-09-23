/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:34:14 by ehammoud          #+#    #+#             */
/*   Updated: 2024/09/22 23:46:23 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GENERAL_H
# define GENERAL_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "mlx.h"
//1.778
# define ASPECT_RATIO 1.6
# define WIN_HEIGHT 860
# define ESC 53
# define CONE 0
# define PLANE 1
# define SPHERE 2
# define CYLINDER 3
# define AMBIENT 4
# define CAMERA 5
# define LIGHT 6
# define INVALID -1
# define ERR_MEM "Memory allocation failed"
# define SUCCESS "Execution Complete!"
# define FAILURE "Unexpected error"
# define PI 3.141592653589793238

typedef struct s_vector
{
	double	x;
	double	y;
	double	z;
}	t_vector;

typedef struct s_vector	t_point;
typedef struct s_vector	t_color;

typedef struct s_matrix_2d
{
	int		rows;
	int		cols;
	double	**array;
}	t_matrix_2d;

typedef struct s_object
{
	int			object_type;
	double		radius;
	double		height;
	t_point		origin;
	t_color		color;
	t_vector	orient;
	t_matrix_2d	trans_matrix;
}	t_object;

typedef struct s_light
{
	int		is_ambient;
	double	brightness;
	t_point	origin;
	t_color	color;
}	t_light;

typedef struct s_camera
{
	t_point		origin;
	t_vector	orient;
	double		focal_len;
	double		fov;
	double		vp_u;
	double		vp_v;
	t_vector	vp_u_diff;
	t_vector	vp_v_diff;
	t_point		top_left_pos;
	t_matrix_2d	trans_matrix;
}	t_camera;

typedef struct s_ray
{
	t_point		origin;
	t_vector	orient;
}	t_ray;

typedef struct s_hit
{
	int			i;
	int			j;
	double		closest;
	t_color		color;
	t_vector	normal;
	t_point		hitp;
	t_object	*obj;
}	t_hit;

typedef struct s_scene
{
	int			cy_cnt;
	int			sp_cnt;
	int			pl_cnt;
	int			co_cnt;
	t_camera	camera;
	t_object	*cones;
	t_object	*planes;
	t_object	*spheres;
	t_object	*cylinders;
	t_light		ambient;
	t_light		light;
}	t_scene;

typedef struct s_main
{
	void	*mlx;
	void	*mw;
	void	*img;
	int		win_width;
	int		win_height;
	double	vp_width;
	double	vp_height;
	int		pixel_bits;
	int		line_bytes;
	int		endian;
	int		busy;
	double	aspect_ratio;
	char	*pixel;
	t_scene	scene;
}	t_main;

void	free_and_exit(t_main *m, char *msg, int status);
void	free_matrix(t_matrix_2d *m);
void	free_scene(t_scene s);
void	free_double_array(double **arr);
int		ft_malloc(void **pointer, int n, int size);
double	**get_objs();

#endif
