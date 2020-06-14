//ESP.cpp
#include "ESP.h"

void ESP::update()
{
	Graphics::beginScene();

	characterInfo_tt* me = Application::game->client->getLocalCharacterInfo();

	
	for (int i = 0; i < 155; i++)
	{
		characterInfo_tt* character = Application::game->client->getCharacterInfo(i);

		// Skip invalid characters
		if (!character->isValid())
			continue;

		// Skip me
		if (me->entityNum == character->entityNum)
			continue;

		// Skip dead entities
		// if (!character->getEntity()->isDead())
		// continue;

		// Get the distance from the character
		float distance = me->pXAnimTree->origin.distance(character->pXAnimTree->origin);

		// Ignore anyone over 20000.0f away
		if (distance > 20000.0f)
			continue;

		//// Ignore my team
		//// if (me.team == character.team)
		//	// continue;

        // Get the players adjusted location
		Vector3 location = ESP::adjustments(
			character->pXAnimTree->origin,
			me->stance,
			character->stance
		);

		// Get the screen position from world origin
		Vector2 position = Math::worldToScreen(
			location,
			me->pXAnimTree->origin,
			Application::game->refdef->displayViewport.width,
			Application::game->refdef->displayViewport.height,
			Application::game->refdef->view.tanHalfFov,
			Application::game->refdef->view.axis
		);

		// Get the screen rectangle size
		Vector2 size = ESP::size(distance);

		// Move position by half size to center
		position.x -= (size.x / 2);
		position.y -= (size.y / 2);

		// Draw
		ESP::draw(character, position, size, distance);
	}
	
	Graphics::endScene();
}

void ESP::draw(characterInfo_tt* character, Vector2 position, Vector2 size, float distance)
{
	// Ignore if off screen
	if (position.x < 0 || position.y < 0 || (position.x + size.x > Application::game->refdef->displayViewport.width) || (position.y + size.y > Application::game->refdef->displayViewport.height))
		return;

	// Draw background
	Graphics::rectangleFilled(position, size, { 0, 0, 0, 100 });

	// Get the colour based off the distance
	ImColor colour = ESP::getColour(distance);

	// Corner Size
	float cornerSize = size.x / 4;

	// Draw name
//	if (distance < 2000.0f)
//		ESP::drawText(position, size, character->getName(), cornerSize, distance, colour);

	// Draw corners
	
	// Top Left
	Graphics::line(position, Vector2(position.x + cornerSize, position.y), colour);
	Graphics::line(position, Vector2(position.x, position.y + cornerSize), colour);

	// Top Right
	Graphics::line(Vector2(position.x + size.x - cornerSize, position.y), Vector2(position.x + size.x, position.y), colour);
	Graphics::line(Vector2(position.x + size.x, position.y), Vector2(position.x + size.x, position.y + cornerSize), colour);

	// Bottom Left
	Graphics::line(Vector2(position.x, position.y + size.y), Vector2(position.x + cornerSize, position.y + size.y), colour);
	Graphics::line(Vector2(position.x, position.y + size.y - cornerSize), Vector2(position.x, position.y + size.y), colour);

	// Bottom Right
	Graphics::line(Vector2(position.x + size.x - cornerSize, position.y + size.y), Vector2(position.x + size.x, position.y + size.y), colour);
	Graphics::line(Vector2(position.x + size.x, position.y + size.y - cornerSize), Vector2(position.x + size.x, position.y + size.y), colour);
}

void ESP::drawText(Vector2 position, Vector2 size, std::string name, float fontSize, float distance, ImColor colour)
{
	// Limit size
	if (fontSize > 32.0f)
		fontSize = 32.0f;

	std::string message = name + "[" + std::to_string((int)distance) + "]";

	Vector2 textSize = Graphics::measureText(message, fontSize);
	position.x += (size.x / 2.0f) - (textSize.x / 2.0f);
	position.y -= textSize.y;

	Graphics::text(position, message, colour, fontSize);
}

Vector2 ESP::size(float distance)
{
	// As the distance increases, the size of width and height decreases
	// This is "Inverse proportion"
	// C = k / D
	// C = Width or Height
	// D = Distance
	// k = C * D

	// At a distance of 272, width is 140 and height is 280
	float pDistance = 272.0f;
	float pWidth = 140.0f;
	float pHeight = 280.0f;

	float kWidth = pDistance * pWidth; // 272 * 140
	float kHeight = pDistance * pHeight; // 272 * 280

	return Vector2(kWidth / distance, kHeight / distance);
}

Vector3 ESP::adjustments(Vector3 position, CharacterStance localStance, CharacterStance playerStance)
{
	position.z -= 25.0f;

	switch (playerStance)
	{
	case CharacterStance::Crouching:
		position.z -= 9.0f;
		break;
	case CharacterStance::Crawling:
	case CharacterStance::Downed:
		position.z -= 22.0f;
		break;
	}

	switch (localStance)
	{
	case CharacterStance::Crouching:
		position.z += 17.5f;
		break;
	case CharacterStance::Crawling:
	case CharacterStance::Downed:
		position.z += 45.0f;
		break;
	}

	return position;
}

ImColor ESP::getColour(float distance)
{
	// Apply limits
	float max = 5000.0f;
	float min = 600.0f;
	if (distance > max)
		distance = max;
	else if (distance < min)
		distance = min;

	distance -= min;

	int green = (int)(distance * (255 / (max - min)));
	return { 255, green, 0, 255 };
}