/* EON - Canvas and Toolkit library
 * Copyright (C) 2008-2009 Jorge Luis Zapata
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef EON_CHECKER_H_
#define EON_CHECKER_H_
/*============================================================================*
 *                                 Events                                     *
 *============================================================================*/
#define EON_CHECKER_SW_CHANGED "swChanged"
#define EON_CHECKER_SH_CHANGED "shChanged"
#define EON_CHECKER_COLOR1_CHANGED "color1Changed"
#define EON_CHECKER_COLOR2_CHANGED "color2Changed"
/*============================================================================*
 *                               Properties                                   *
 *============================================================================*/
extern Ekeko_Property_Id EON_CHECKER_SW;
extern Ekeko_Property_Id EON_CHECKER_SH;
extern Ekeko_Property_Id EON_CHECKER_COLOR1;
extern Ekeko_Property_Id EON_CHECKER_COLOR2;
/*============================================================================*
 *                                 Class                                      *
 *============================================================================*/
typedef struct _Eon_Checker_Private Eon_Checker_Private;
struct _Eon_Checker
{
	Eon_Paint_Square parent;
	Eon_Checker_Private *prv;
};
/*============================================================================*
 *                                Functions                                   *
 *============================================================================*/
EAPI Eon_Checker * eon_checker_new(Eon_Document *d);

EAPI void eon_checker_color1_set(Eon_Checker *sq, Eon_Color color);
EAPI Eon_Color eon_checker_color1_get(Eon_Checker *sq);

EAPI void eon_checker_color2_set(Eon_Checker *sq, Eon_Color color);
EAPI Eon_Color eon_checker_color2_get(Eon_Checker *sq);

EAPI void eon_checker_sw_get(Eon_Checker *ch, Eon_Coord *sw);
EAPI void eon_checker_sw_rel_set(Eon_Checker *s, int sw);
EAPI void eon_checker_sw_set(Eon_Checker *s, int sw);

EAPI void eon_checker_sh_get(Eon_Checker *s, Eon_Coord *sh);
EAPI void eon_checker_sh_set(Eon_Checker *s, int sh);
EAPI void eon_checker_sh_rel_set(Eon_Checker *s, int sh);

#endif /* EON_CHECKER_H_ */
