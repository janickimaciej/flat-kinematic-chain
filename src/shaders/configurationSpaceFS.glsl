#version 420 core

in float angle1Rad;
in float angle2Rad;

uniform float kinematicChainLength1;
uniform float kinematicChainLength2;
uniform vec2 obstaclePos;
uniform vec2 obstacleSize;

out vec4 outColor;

bool intersectsObstacle();
bool intersectsObstacle(vec2 start, vec2 segment);
bool intersectsLine(vec2 start, vec2 segment, float lineX, float lineYMin, float lineYMax);

void main()
{
	if (!intersectsObstacle()) discard;
	outColor = vec4(0.7, 0, 0, 1);
}

bool intersectsObstacle()
{
	vec2 obstaclePosMin = obstaclePos - obstacleSize / 2.0;
	vec2 obstaclePosMax = obstaclePos + obstacleSize / 2.0;

	if (obstaclePosMin.x * obstaclePosMax.x <= 0 && obstaclePosMin.y * obstaclePosMax.y <= 0)
	{
		return true;
	}

	vec2 intermediatePoint = kinematicChainLength1 * vec2(cos(angle1Rad), sin(angle1Rad));
	return intersectsObstacle(vec2(0, 0), intermediatePoint) ||
		intersectsObstacle(intermediatePoint,
			kinematicChainLength2 * vec2(cos(angle1Rad + angle2Rad), sin(angle1Rad + angle2Rad)));
}

bool intersectsObstacle(vec2 start, vec2 segment)
{

	vec2 obstaclePosMin = obstaclePos - obstacleSize / 2.0;
	vec2 obstaclePosMax = obstaclePos + obstacleSize / 2.0;

	if (segment.x != 0)
	{
		if (intersectsLine(start, segment, obstaclePosMin.x, obstaclePosMin.y, obstaclePosMax.y) ||
			intersectsLine(start, segment, obstaclePosMax.x, obstaclePosMin.y, obstaclePosMax.y))
		{
			return true;
		}
	}

	if (segment.y != 0)
	{
		if (intersectsLine(vec2(start.y, start.x), vec2(segment.y, segment.x), obstaclePosMin.y,
				obstaclePosMin.x, obstaclePosMax.x) ||
			intersectsLine(vec2(start.y, start.x), vec2(segment.y, segment.x), obstaclePosMax.y,
				obstaclePosMin.x, obstaclePosMax.x))
		{
			return true;
		}
	}

	return false;
}

bool intersectsLine(vec2 start, vec2 segment, float lineX, float lineYMin, float lineYMax)
{
	const float eps = 0;

	float t = (lineX - start.x) / segment.x;
	if (t < -eps || t > 1 + eps)
	{
		return false;
	}
	
	float yIntersection = start.y + t * segment.y;
	return yIntersection > lineYMin - eps && yIntersection < lineYMax + eps;
}
