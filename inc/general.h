/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:34:14 by ehammoud          #+#    #+#             */
/*   Updated: 2024/09/15 16:10:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include "mlx.h"

# define ASPECT_RATIO 1.778
# define WIN_HEIGHT 512
# define ESC 53
# define CONE 0
# define PLANE 1
# define SPHERE 2
# define CYLINDER 3
# define AMBIENT 4
# define CAMERA 5
# define LIGHT 6
# define INVALID -1

typedef struct	s_main
{
	void	*mlx;
	void	*mw;
	void	*img;
	int		win_width;
	int		win_height;
	int		pixel_bits;
	int		line_bytes;
	int		endian;
	char	*pixel;
	t_scene	scene;
}	t_main;

typedef struct s_vector
{
	float	x;
	float	y;
	float	z;
}	t_vector;

typedef struct s_vector	t_point;
typedef struct s_vector	t_color;

typedef struct s_matrix_2d
{
	int		rows;
	int		cols;
	float	**array;
}	t_matrix_2d;

typedef struct s_object
{
	int			object_type;
	float		radius;
	float		height;
	t_point		origin;
	t_color		color;
	t_vector	orient;
	t_matrix_2d	trans_matrix;
}	t_object;

typedef struct s_light
{
	int		is_ambient;
	float	brightness;
	t_point	origin;
	t_color	color;
}	t_light;

typedef struct s_camera
{
	float		fov;
	t_point		origin;
	t_vector	orient;
	t_matrix_2d	trans_matrix;
}	t_camera;

typedef struct s_ray
{
	t_point		origin;
	t_vector	orient;
}	t_ray;

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
	t_light		ambient_light;
	t_light		light_source;
}	t_scene;

void	free_and_exit(t_main *m, char *msg, int status);

#endif