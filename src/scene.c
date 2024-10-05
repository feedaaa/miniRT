/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 21:21:30 by ehammoud          #+#    #+#             */
/*   Updated: 2024/10/05 21:24:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"

static void	setup_shapes(t_main *m, int cnt[4])
{
	if (!m)
		return ;
	m->scene.co_cnt = 0;
	m->scene.pl_cnt = 0;
	m->scene.sp_cnt = 0;
	m->scene.cy_cnt = 0;
	create_shapes_arr(m, (void **)&m->scene.cones, cnt[CONE], CONE);
	create_shapes_arr(m, (void **)&m->scene.planes, cnt[PLANE], PLANE);
	create_shapes_arr(m, (void **)&m->scene.spheres, cnt[SPHERE], SPHERE);
	create_shapes_arr(m, (void **)&m->scene.cylinders, cnt[CYLINDER], CYLINDER);
}

void	init_scene(t_main *m, t_scene *s, double **objs)
{
	int	i;
	int	shape_cnt[4];

	count_shapes(shape_cnt, objs);
	setup_shapes(m, shape_cnt);
	i = -1;
	while (objs[++i][0] != INVALID)
	{
		if (is_equal(objs[i][0], CONE))
			read_shape_values(m, &s->cones[s->co_cnt++], objs[i]);
		if (is_equal(objs[i][0], PLANE))
			read_shape_values(m, &s->planes[s->pl_cnt++], objs[i]);
		if (is_equal(objs[i][0], SPHERE))
			read_shape_values(m, &s->spheres[s->sp_cnt++], objs[i]);
		if (is_equal(objs[i][0], CYLINDER))
			read_shape_values(m, &s->cylinders[s->cy_cnt++], objs[i]);
		if (is_equal(objs[i][0], CAMERA))
			read_camera_values(m, &s->camera, objs[i]);
		if (is_equal(objs[i][0], AMBIENT))
			read_light_values(m, &s->ambient, objs[i]);
		if (is_equal(objs[i][0], LIGHT))
			read_light_values(m, &s->light, objs[i]);
	}
}

void	setup_scene(t_main *m, t_scene *s)
{
	s->camera.focal_len = 1;
	m->vp_width = 2.0 * tan((s->camera.fov * PI) / 360) / s->camera.focal_len;
	m->vp_height = m->vp_width / m->aspect_ratio;
	s->camera.vp_u = m->vp_width / m->win_width;
	s->camera.vp_v = m->vp_height / m->win_height;
	assign(&s->camera.top_left_pos,
		-(m->vp_width / 2.0) + (s->camera.vp_u / 2.0),
		(m->vp_height / 2.0) - (s->camera.vp_v / 2.0),
		-s->camera.focal_len);
}

int	interrupted(t_main *m, t_hit *h)
{
	int		u;
	t_ray	ray;
	t_hit	hit;

	copy_vector(&ray.origin, &h->hitp);
	vector_op(&ray.orient, &m->scene.light.origin, '-', &h->hitp);
	normalize(&ray.orient);
	u = 0;
	while (u < m->scene.sp_cnt || u < m->scene.pl_cnt
		|| u < m->scene.cy_cnt || u < m->scene.co_cnt)
	{
		if (u < m->scene.sp_cnt && hit_sphere(ray, m->scene.spheres[u], &hit))
			return (1);
		if (u < m->scene.pl_cnt && hit_plane(ray, m->scene.planes[u], &hit))
			return (1);
		if (u < m->scene.cy_cnt
			&& hit_cylinder(ray, m->scene.cylinders[u], &hit))
			return (1);
		if (u < m->scene.co_cnt && hit_cone(ray, m->scene.cones[u], &hit))
			return (1);
		u++;
	}
	return (0);
}
