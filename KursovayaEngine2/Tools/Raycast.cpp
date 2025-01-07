#include"Raycast.h"
/*
RaycastNS::RaycastResultStruct RaycastNS::LaunchRay(const ModelNamespace::InstanceStruct& inst, RayDataStruct rayData) {
	RaycastResultStruct raycastRes;

	{//fast checks
		Vector3F instPosForRay = inst.Pos - rayData.StartPos;

		float distToInstSQ = instPosForRay.LengthSQ();
		float instCircleRadiusSQ = (inst.ModelBase.Size * inst.Scale).LengthSQ() / 2;

		//check for distance
		if (rayData.Length >= 0.f && (distToInstSQ - instCircleRadiusSQ) > rayData.Length * rayData.Length) return raycastRes;

		//check for closest distance from center to direction vector with circle
		float dotPR = instPosForRay.Dot(rayData.Direction);
		float closestLenToCircleSQ = distToInstSQ * (1 - dotPR * dotPR);
		if (closestLenToCircleSQ > instCircleRadiusSQ) return raycastRes;
	}

	
	Matrix33F InversedInstRotMat = inst.Rot.GetInversedMatrix(inst.Rot.GetDeterminant());
	rayData.StartPos = (InversedInstRotMat * (rayData.StartPos - inst.Pos)) / inst.Scale;
	rayData.Direction = (InversedInstRotMat * rayData.Direction) / inst.Scale;
	if (rayData.Length >= 0.f) rayData.Length /= inst.Scale.Length();

	//some triangles of instance can actually collide with the ray... gotta check all of those
	for (unsigned int ti = 0; ti < inst.ModelBase.TrianglesPoints.size() / 3; ti++) {
		Vector3F p1 = inst.ModelBase.TrianglesPoints[ti * 3 + 0];
		Vector3F p2 = inst.ModelBase.TrianglesPoints[ti * 3 + 1];
		Vector3F p3 = inst.ModelBase.TrianglesPoints[ti * 3 + 2];
		Vector3F normalFL = (p2 - p1).CrossFL(p3 - p1);
		

		//check if normal is looking at oppisite of ray direction
		if (normalFL.DotFL(rayData.Direction) < 0) continue;

		//projection on triangle's plane
		//Vector3F proj = rayData.StartPos - normalFL * (normalFL.DotFL(rayData.StartPos - p1) / normalFL.LengthSQ());
		Vector3F proj = rayData.StartPos - rayData.Direction * ((rayData.StartPos - p1).DotFL(normalFL) / normalFL.DotFL(rayData.Direction));

		float distToProjSQ = (proj - rayData.StartPos).LengthSQ();

		//check if triangle shouldnt be ignored by distance
		if ((rayData.Length >= 0.f && rayData.Length * rayData.Length < distToProjSQ) || (raycastRes.Hit && raycastRes.Dist * raycastRes.Dist < distToProjSQ))
			continue;

		//check if projected point is in triangle
		{
			float dot_PP1_P2P1 = (proj - p1).Dot(p2 - p1);
			float dot_PP1_P3P1 = (proj - p1).Dot(p3 - p1);
			float dot_P2P1_P3P1 = (p2 - p1).Dot(p3 - p1);
			if (dot_P2P1_P3P1 < dot_PP1_P2P1 && dot_P2P1_P3P1 < dot_PP1_P3P1) {
				float dot_PP3_P1P3 = (proj - p3).Dot(p1 - p3);
				float dot_P2P3_P1P3 = (p2 - p3).Dot(p1 - p3);
				if (dot_PP3_P1P3 > dot_P2P3_P1P3) {
					raycastRes.Hit = true;
					raycastRes.Dist = sqrtf(distToProjSQ);
					raycastRes.HitPos = proj;
					raycastRes.TriangleInd = ti;
				}
				else continue;
			}
			else continue;
		}
	}


	raycastRes.HitPos = inst.Rot * (raycastRes.HitPos * inst.Scale) + inst.Pos;
	raycastRes.Dist *= inst.Scale.Length();

	return raycastRes;
}
*/