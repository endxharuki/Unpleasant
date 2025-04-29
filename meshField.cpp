
//main.hを一番上にしましょう。
#include "main.h"
#include "meshField.h"
#include "gameScene.h"
#include "scene.h"
#include "player.h"
#include "transform3DAnimationComponent.h"
#include "texturePool.h"
#include "audio.h"


float g_FieldHeight[FIELD_X][FIELD_Z] =
{
	{0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,4.0f,5.0f,0.0f,0.0f,0.0f},
	{0.0f,5.0f,4.0f,7.0f,8.0f,10.0f,9.5f,7.0f,5.0f,5.0f,6.0f,4.0f,5.0f,6.0f,6.0f,6.0f,7.0f,6.0f,6.0f,5.0f,4.0f,6.5f,8.0f,8.0f,6.0f,4.0f,5.0f,5.0f,5.0f,6.0f,7.0f,5.0f,6.0f,6.0f,4.0f,7.0f,8.0f,9.5f,11.0f,13.0f,16.0f,15.0f,14.0f,13.0f,0.0f,0.0f},
	{0.0f,4.0f,2.0f,1.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,4.0f,2.0f,1.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,8.0f,17.0f,11.0f,10.0f,12.0f,0.0f},
	{0.0f,5.0f,5.0f,5.0f,7.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,5.0f,0.0f},
	{0.0f,4.0f,5.0f,6.0f,7.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,6.0f,0.0f},
	{0.0f,4.0f,5.0f,5.0f,4.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,7.0f,0.0f},
	{0.0f,7.0f,2.0f,4.0f,3.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,8.0f,0.0f},
	{0.0f,6.0f,2.0f,4.0f,4.0f,2.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,10.0f,0.0f},
	{0.0f,6.0f,2.0f,3.0f,2.0f,1.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,9.0f,0.0f},
	{0.0f,6.0f,2.0f,0.0f,1.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,6.0f,0.0f},
	{0.0f,5.0f,3.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,5.0f,0.0f},
	{0.0f,5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,7.0f,0.0f},
	{0.0f,6.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,4.0f,0.0f},
	{0.0f,5.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,5.0f,0.0f},
	{0.0f,7.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,5.0f,0.0f},
	{0.0f,7.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,6.0f,0.0f},
	{0.0f,6.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,7.0f,0.0f},
	{0.0f,5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,10.0f,0.0f},
	{0.0f,5.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,6.0f,0.0f},
	{0.0f,5.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,1.0f,1.0f,1.0f,6.0f,6.0f,7.0f,5.0f,4.0f,5.0f,5.0f,5.0f,5.0f,7.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,5.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,6.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,6.0f,0.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,7.0f,0.0f},
	{0.0f,5.0f,4.0f,5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,5.0f,6.0f,6.0f,6.0f,7.0f,6.0f,6.0f,5.0f,5.0f,7.0f,0.0f},
	{0.0f,4.0f,2.0f,1.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,8.0f,0.0f},
	{0.0f,5.0f,5.0f,5.0f,7.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,6.0f,0.0f},
	{0.0f,4.0f,5.0f,6.0f,7.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,5.0f,0.0f},
	{0.0f,4.0f,5.0f,5.0f,4.0f,3.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,4.0f,0.0f},
	{0.0f,7.0f,2.0f,4.0f,3.0f,3.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,6.0f,0.0f},
	{0.0f,6.0f,2.0f,4.0f,4.0f,2.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,3.0f,1.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,5.0f,0.0f},
	{0.0f,6.0f,2.0f,3.0f,2.0f,1.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,2.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,4.0f,4.0f,6.0f,0.0f},
	{0.0f,6.0f,2.0f,0.0f,1.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,2.0f,1.0f,1.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,5.0f,5.0f,4.0f,0.0f},
	{0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,7.0f,0.0f},
	{0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,6.0f,0.0f},
	{0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,8.0f,0.0f},
	{0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,9.0f,0.0f},
	{0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,10.0f,0.0f},
	{0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,11.0f,0.0f},
	{0.0f,6.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,9.0f,0.0f},
	{0.0f,5.0f,4.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,7.0f,7.0f,7.0f,0.0f},
	{0.0f,5.0f,5.0f,3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,0.0f,0.0f,0.0f,0.0f,5.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,6.0f,6.0f,6.0f,0.0f},
	{0.0f,5.0f,4.0f,5.0f,6.0f,6.0f,7.0f,7.0f,5.0f,5.0f,5.0f,4.0f,6.0f,6.0f,7.0f,5.0f,4.0f,5.0f,5.0f,5.0f,0.0f,0.0f,0.0f,0.0f,5.0f,4.0f,5.0f,6.0f,6.0f,7.0f,7.0f,5.0f,5.0f,5.0f,4.0f,6.0f,6.0f,7.0f,5.0f,4.0f,5.0f,5.0f,5.0f,0.0f,5.0f,0.0f},
	{0.0f,9.0f,5.0f,7.0f,6.0f,7.0f,9.0f,8.0f,9.0f,11.0f,12.0f,11.0f,9.0f,8.0f,5.0f,7.0f,7.0f,7.0f,7.0f,7.0f,8.0f,9.0f,13.0f,18.0f,17.0f,22.0f,29.0f,32.0f,33.0f,30.0f,27.0f,24.0f,21.0f,19.0f,18.0f,14.0f,12.0f,10.0f,7.0f,6.0f,8.0f,9.0f,9.0f,10.0f,8.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,2.0f,2.0f,3.0f,4.0f,6.0f,8.0f,7.0f,5.0f,5.0f,4.0f,3.0f,4.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,1.0f,2.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,1.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,2.0f,2.0f,3.0f,4.0f,6.0f,8.0f,7.0f,5.0f,5.0f,4.0f,3.0f,4.0f,2.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f}
	

};


void MeshField::Init()
{
	m_FieldX = 100;
	m_FieldZ = 100;
	m_VertexSpace = 10.0f;

	m_VertexData.resize(m_FieldX, std::vector<VERTEX_3D>(m_FieldZ));

	//頂点バッファ生成
	{
		//for (int x = 0; x < m_FieldX; x++)//
		//{
		//	for (int z = 0; z < m_FieldZ; z++)//
		//	{
		//		m_Vertex[x][z].Position = XMFLOAT3((x - 10) * m_VertexSpace, 0.0f, (z - 10) * -m_VertexSpace);
		//		m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		//		m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		//		m_Vertex[x][z].TexCoord = XMFLOAT2(x, z);
		//	}
		//}

		for (int x = 0; x < m_FieldX; x++)//
		{
			for (int z = 0; z < m_FieldZ; z++)//
			{
				m_VertexData[x][z].Position = XMFLOAT3((x - m_FieldX) * m_VertexSpace, 0.0f, (z - m_FieldZ) * -m_VertexSpace);
				m_VertexData[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
				m_VertexData[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_VertexData[x][z].TexCoord = XMFLOAT2(x, z);
			}
		}

		//法線ベクトル計算
		for (int x = 1; x < (m_FieldX - 1); x++)//
		{
			for (int z = 1; z < (m_FieldZ - 1); z++)//
			{
				XMFLOAT3 vx, vz, vn;
				vx.x = m_VertexData[x + 1][z].Position.x - m_VertexData[x - 1][z].Position.x;
				vx.y = m_VertexData[x + 1][z].Position.y - m_VertexData[x - 1][z].Position.y;
				vx.z = m_VertexData[x + 1][z].Position.z - m_VertexData[x - 1][z].Position.z;

				vz.x = m_VertexData[x][z - 1].Position.x - m_VertexData[x][z + 1].Position.x;
				vz.y = m_VertexData[x][z - 1].Position.y - m_VertexData[x][z + 1].Position.y;
				vz.z = m_VertexData[x][z - 1].Position.z - m_VertexData[x][z + 1].Position.z;

				//外積
				vn.x = vz.y * vx.z - vz.z * vx.y;
				vn.y = vz.z * vx.x - vz.x * vx.z;
				vn.z = vz.x * vx.y - vz.y * vx.x;

				//正規化(長さ１にする)
				float len = sqrtf(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
				vn.x /= len;
				vn.y /= len;
				vn.z /= len;

				m_VertexData[x][z].Normal = vn;

			}

		}

		std::vector<VERTEX_3D> vertexFlatData;

		//二次元配列をフラットにする
		for (int x = 0; x < m_FieldX; x++)
		{
			for (int z = 0; z < m_FieldZ; z++)
			{
				vertexFlatData.push_back(m_VertexData[x][z]);
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertexFlatData.size();//
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertexFlatData.data();

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ
	{

		//unsigned int index[((FIELD_X + 1) * 2) * (FIELD_Z - 1) - 2];//

		int i = 0;
		for (int x = 0; x < m_FieldX - 1; x++)//
		{
			for (int z = 0; z < m_FieldZ; z++)//
			{
				m_Index.push_back(x * m_FieldX + z);
				//index[i] = x * m_FieldX + z;//
				//i++;

				m_Index.push_back((x + 1) * (m_FieldX)+z);
				//index[i] = (x + 1) * (m_FieldX) + z;//
				//i++;
			}
			if (x == (m_FieldX - 2))//
				break;
			m_Index.push_back((x + 1) * (m_FieldX)+(m_FieldZ - 1));
			//index[i] = (x + 1) * (m_FieldX) + (m_FieldZ - 1);//
			//i++;

			m_Index.push_back((x + 1) * (m_FieldX));
			//index[i] = (x + 1) * (m_FieldX);//
			//i++;
		}
		//D3D11_BUFFER_DESC bd;
		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(unsigned int) * ((m_FieldX + 1) * 2) * (m_FieldZ - 1) - 2;//
		//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//bd.CPUAccessFlags = 0;

		//D3D11_SUBRESOURCE_DATA sd;
		//ZeroMemory(&sd, sizeof(sd));
		//sd.pSysMem = index;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * m_Index.size();//
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Index.data();


		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}


	LoadMeshField("field1.csv");

	m_Texture = TexturePool::GetInstance()->GetTexture("Jimen");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	Audio* audio = AddComponent<Audio>();
	audio->InitMaster();

	audio->Load("asset\\Audio\\gameBGM.wav");

	audio->Play(1);

	audio->SetVolume(0.5f);

}

void MeshField::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void MeshField::Update()
{

}

void MeshField::Draw()
{


	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//シェーダー設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//ワールドマトリクス設定
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//Renderer::SetDepthEnable(false);

	//ポリゴン設定
	//Renderer::GetDeviceContext()->Draw(21 * 21, 0);
	Renderer::GetDeviceContext()->DrawIndexed(m_Index.size(), 0, 0);

	//Renderer::SetDepthEnable(true);
	/*DrawImGui();*/

}

void MeshField::DrawImGui()
{

	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();

	int x, z;
	//ブロック番号算出
	x = playerPos.x / 5.0f + 10.0f;		//Initで設定してる値
	z = playerPos.z / -5.0f + 10.0f;

	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = m_Vertex[x][z].Position;
	pos1 = m_Vertex[x + 1][z].Position;
	pos2 = m_Vertex[x][z + 1].Position;
	pos3 = m_Vertex[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x =  playerPos.x - pos1.x;
	v1p.y =  playerPos.y - pos1.y;
	v1p.z =  playerPos.z - pos1.z;

	//外積
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;
	XMFLOAT3 n;
	if (cy > 0.0f)
	{
		//左上ポリゴン
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		//外積
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		//右上ポリゴン
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		//外積
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	////高さ取得
	//py = -((playerPos.x - pos1.x) * n.x + (playerPos.z - pos1.z) * n.z) / n.y + pos1.y;

	//{

	//	ImGui::Begin("MeshField");                          // Create a window called "Hello, world!" and append into it.

	//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	//	ImGui::Text("BlockNumber%d,%d", x, z);
	//	ImGui::Text("Gaiseki = %.1f", cy);
	//	ImGui::Text("Hight = %.1f", py);


	//	ImGui::End();

	//}

}

void MeshField::UpdateVertexBuffer()
{

	std::vector<VERTEX_3D> vertexFlatData;

	//二次元配列をフラットにする
	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			vertexFlatData.push_back(m_VertexData[x][z]);
		}
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, vertexFlatData.data(), sizeof(VERTEX_3D) * vertexFlatData.size());
	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void MeshField::ReCreateVertexAndIndexBuffer()
{

	std::vector<VERTEX_3D> vertexFlatData;

	//二次元配列をフラットにする
	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			vertexFlatData.push_back(m_VertexData[x][z]);
		}
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertexFlatData.size();//
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertexFlatData.data();

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);


	m_Index.clear();
	//インデックスバッファ
	{

		//unsigned int index[((FIELD_X + 1) * 2) * (FIELD_Z - 1) - 2];//

		int i = 0;
		for (int x = 0; x < m_FieldX - 1; x++)//
		{
			for (int z = 0; z < m_FieldZ; z++)//
			{
				m_Index.push_back(x * m_FieldX + z);
				//index[i] = x * m_FieldX + z;//
				//i++;

				m_Index.push_back((x + 1) * (m_FieldX)+z);
				//index[i] = (x + 1) * (m_FieldX) + z;//
				//i++;
			}
			if (x == (m_FieldX - 2))//
				break;
			m_Index.push_back((x + 1) * (m_FieldX)+(m_FieldZ - 1));
			//index[i] = (x + 1) * (m_FieldX) + (m_FieldZ - 1);//
			//i++;

			m_Index.push_back((x + 1) * (m_FieldX));
			//index[i] = (x + 1) * (m_FieldX);//
			//i++;
		}
		//D3D11_BUFFER_DESC bd;
		//ZeroMemory(&bd, sizeof(bd));
		//bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.ByteWidth = sizeof(unsigned int) * ((m_FieldX + 1) * 2) * (m_FieldZ - 1) - 2;//
		//bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//bd.CPUAccessFlags = 0;

		//D3D11_SUBRESOURCE_DATA sd;
		//ZeroMemory(&sd, sizeof(sd));
		//sd.pSysMem = index;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * m_Index.size();//
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Index.data();


		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

}

void MeshField::LoadMeshField(const std::string& filename)
{

	m_FileName = filename;
	std::ifstream file(filename);
	if (!file) return;

	std::string line;
	std::getline(file, line);  //ヘッダー行を読み飛ばす (x,y,z)

	//フィールドのサイズを取得
	std::getline(file, line);
	std::stringstream ss(line);
	std::string fieldxStr, fieldzStr;

	std::getline(ss, fieldxStr, ',');
	std::getline(ss, fieldzStr, ',');

	m_FieldX = std::stof(fieldxStr);
	m_FieldZ = std::stof(fieldzStr);

	m_VertexData.clear();
	m_VertexData.resize(m_FieldX, std::vector<VERTEX_3D>(m_FieldZ));

	std::vector<VERTEX_3D> vertices;
	while (std::getline(file, line))
	{
		VERTEX_3D v;
		std::stringstream ss(line);
		std::string xStr, yStr, zStr;
		std::string xNormal, yNormal, zNormal;

		//座標
		std::getline(ss, xStr, ',');
		std::getline(ss, yStr, ',');
		std::getline(ss, zStr, ',');
		//法線情報
		std::getline(ss, xNormal, ',');
		std::getline(ss, yNormal, ',');
		std::getline(ss, zNormal, ',');


		v.Position.x = std::stof(xStr);
		v.Position.y = std::stof(yStr);
		v.Position.z = std::stof(zStr);

		v.Normal.x = std::stof(xNormal);
		v.Normal.y = std::stof(yNormal);
		v.Normal.z = std::stof(zNormal);

		vertices.push_back(v);
	}
	file.close();

	// ロードしたデータを `m_Vertex` に反映
	int index = 0;
	for (int x = 0; x < m_FieldX; x++)
	{
		for (int z = 0; z < m_FieldZ; z++)
		{
			if (index < vertices.size())
			{
				m_VertexData[x][z] = vertices[index];

				m_VertexData[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_VertexData[x][z].TexCoord = XMFLOAT2(x, z);
				index++;
			}
		}
	}

	ReCreateVertexAndIndexBuffer();

}

float MeshField::GetHeight(XMFLOAT3 pos)
{
	int x, z;
	//ブロック番号算出
	x = pos.x / 5.0f + (m_FieldX / 2);		//Initで設定してる値
	z = pos.z / -5.0f + (m_FieldZ / 2);

	int xLen = m_VertexData.size();
	if (x >= xLen -1)
	{
		return 0.0f;
	}

	int zLen = m_VertexData[x].size();

	if (z >= zLen-1)
	{
		return 0.0f;
	}


	XMFLOAT3 pos0, pos1, pos2, pos3;
	pos0 = m_VertexData[x][z].Position;
	pos1 = m_VertexData[x + 1][z].Position;
	pos2 = m_VertexData[x][z + 1].Position;
	pos3 = m_VertexData[x + 1][z + 1].Position;

	XMFLOAT3 v12, v1p;
	v12.x = pos2.x - pos1.x;
	v12.y = pos2.y - pos1.y;
	v12.z = pos2.z - pos1.z;

	v1p.x = pos.x - pos1.x;
	v1p.y = pos.y - pos1.y;
	v1p.z = pos.z - pos1.z;

	//外積
	float cy = v12.z * v1p.x - v12.x * v1p.z;

	float py;
	XMFLOAT3 n;
	if (cy > 0.0f)
	{
		//左上ポリゴン
		XMFLOAT3 v10;
		v10.x = pos0.x - pos1.x;
		v10.y = pos0.y - pos1.y;
		v10.z = pos0.z - pos1.z;

		//外積
		n.x = v10.y * v12.z - v10.z * v12.y;
		n.y = v10.z * v12.x - v10.x * v12.z;
		n.z = v10.x * v12.y - v10.y * v12.x;
	}
	else
	{
		//右上ポリゴン
		XMFLOAT3 v13;
		v13.x = pos3.x - pos1.x;
		v13.y = pos3.y - pos1.y;
		v13.z = pos3.z - pos1.z;

		//外積
		n.x = v12.y * v13.z - v12.z * v13.y;
		n.y = v12.z * v13.x - v12.x * v13.z;
		n.z = v12.x * v13.y - v12.y * v13.x;
	}

	//高さ取得
	py = -((pos.x - pos1.x) * n.x + (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}
