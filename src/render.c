/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 11:28:33 by ehammoud          #+#    #+#             */
/*   Updated: 2024/09/26 02:13:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"

void	color_pixel(t_main *m, int x, int y, unsigned int color)
{
	int	pixel_index;

	if (!m || !m->pixel || x < 0 || y < 0 || x >= m->win_height || y >= m->win_width)
		return ;
	pixel_index = (x * m->line_bytes) + (y * 4);
	if (m->pixel_bits != 32)
		color = mlx_get_color_value(m->mlx, color);
	if (m->endian == 1)
	{
		m->pixel[pixel_index + 0] = (color >> 24);
		m->pixel[pixel_index + 1] = (color >> 16) & 0xFF;
		m->pixel[pixel_index + 2] = (color >> 8) & 0xFF;
		m->pixel[pixel_index + 3] = (color) & 0xFF;
	}
	else if (m->endian == 0)
	{
		m->pixel[pixel_index + 0] = (color) & 0xFF;
		m->pixel[pixel_index + 1] = (color >> 8) & 0xFF;
		m->pixel[pixel_index + 2] = (color >> 16) & 0xFF;
		m->pixel[pixel_index + 3] = (color >> 24);
	}
}

static unsigned int	color_to_hex(t_color color)
{
	if (color.x > 255)
		color.x = 255;
	if (color.y > 255)
		color.y = 255;
	if (color.z > 255)
		color.z = 255;
	if (color.x < 0)
		color.x = 0;
	if (color.y < 0)
		color.y = 0;
	if (color.z < 0)
		color.z = 0;
	return ((((unsigned int)color.x) << 16) +
		(((unsigned int)color.y) << 8) +
			(((unsigned int)color.z)));
}

void	light_up(t_main *m, t_hit *hit)
{
	double		light;
	double		li_dot;
	t_vector	to_light;

	if (!m || !hit)
		return ;
	light = m->scene.ambient.brightness;
	vector_op(&to_light, &m->scene.light.origin, '-', &hit->hitp);
	// vector_op(&to_light, &hit->hitp, '-', &m->scene.light.origin);
	// if (hit->i % 100 == 0 && hit->j % 100 == 0)
	// 	printf("----- %lf, %lf, %lf\n", hit->hitp.x, hit->hitp.y, hit->hitp.z);
	li_dot = dot(to_light, hit->normal);
	// if (hit->obj->object_type == PLANE && hit->i % 100 == 0 && hit->j % 100 == 0)
	// 	printf("~ %lf, %lf, %lf ~~ %lf <> %lf\n", hit->hitp.x, hit->hitp.y, hit->hitp.z, li_dot, light);
	if (li_dot > 0)
		light += m->scene.light.brightness * li_dot /
			(sqrt(dot(hit->normal, hit->normal)) * sqrt(dot(to_light, to_light)));
	// if (hit->obj->object_type == PLANE && hit->i % 100 == 0 && hit->j % 100 == 0)
	// 	printf("%lf, %lf, %lf, <> %lf\n", hit->obj->origin.x, hit->obj->origin.y, hit->obj->origin.z, light);
	scalar_op(&hit->color, &hit->color, '*', light);
}

void	hit_objects(t_main *m, t_ray *ray, int i, int j)
{
	int		u;
	t_hit	hit;

	hit.i = i;
	hit.j = j;
	hit.closest = -1;
	u = -1;
	while (++u < m->scene.co_cnt)
		hit_cone(ray, &m->scene.cones[u], &hit);
	u = -1;
	while (++u < m->scene.sp_cnt)
		hit_sphere(&m->scene.camera, *ray, m->scene.spheres[u], &hit);
	u = -1;
	while (++u < m->scene.pl_cnt)
		hit_plane(ray, &m->scene.planes[u], &hit);
	u = -1;
	while (++u < m->scene.cy_cnt)
		hit_cylinder(ray, &m->scene.cylinders[u], &hit);
	if (hit.closest > -1)
	{
		// light_up(m, &hit);
		(void)color_to_hex;
		color_pixel(m, i, j, color_to_hex(hit.color));
	}
}

void	render_scene(t_main *m)
{
	t_ray	ray;
	int		i;
	int		j;

	if (m->busy)
		return ;
	m->busy = 1;
	i = -1;
	// printf("START %lf, %lf, %lf\n", m->scene.camera.top_left_pos.x, m->scene.camera.top_left_pos.y, m->scene.camera.top_left_pos.z);
	while (++i < m->win_height)
	{
		j = -1;
		while (++j < m->win_width)
		{
			copy_vector(&ray.origin, &m->scene.camera.origin);
			copy_vector(&ray.orient, &m->scene.camera.top_left_pos);
			ray.orient.x += j * m->scene.camera.vp_u;
			ray.orient.y -= i * m->scene.camera.vp_v;
			normalize(&ray.orient);
			// transform_vector(&m->scene.camera.trans_matrix, &ray.origin, 1);
			// transform_vector(&m->scene.camera.trans_matrix, &ray.orient, 0);
			// if (i % 100 == 0 && j % 100 == 0 && i == j)
			// 	printf("%lf, %lf\n", ray.orient.x, ray.orient.y);
			// if (!i && !j)
			// 	printf("START AFTER %lf, %lf, %lf\n", ray.orient.x, ray.orient.y, ray.orient.z);
			// if (i == m->win_height - 1 && j == m->win_width - 1)
			// 	printf("END AFTER %lf, %lf, %lf\n", ray.orient.x, ray.orient.y, ray.orient.z);
			// if (is_equal(ray.orient.x, 0))
			// {
			// 	for (int w=i; w < i + 5;w++)
			// 		for(int q=j; q < j + 5; q++)
			// 			color_pixel(m, w, q, 0xffff);
			// }
			// if (is_equal(ray.orient.y, 0))
			// {
			// 	for (int w=i; w < i + 5;w++)
			// 		for(int q=j; q < j + 5; q++)
			// 			color_pixel(m, w, q, 0xffffff);
			// }
			hit_objects(m, &ray, i, j);
		}
	}
	// printf("o2: %lf, %lf, %lf\n", sp.origin.x, sp.origin.y, sp.origin.z);
	m->busy = 0;
}

			// if (is_equal(ray.orient.x, ray.orient.y))
			// {
			// 	for (int w=i; w < i + 5;w++)
			// 		for(int q=j; q < j + 5; q++)
			// 			color_pixel(m, w, q, 0xffff);
			// }
			// if (is_equal(ray.orient.x, -ray.orient.y))
			// {
			// 	for (int w=i; w < i + 5;w++)
			// 		for(int q=j; q < j + 5; q++)
			// 			color_pixel(m, w, q, 0xffff);
			// }