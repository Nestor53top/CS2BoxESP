#pragma once

struct Vector3 { float x, y, z; };
struct Vector2 { float x, y; };

enum BoneIndices : int {
    PELVIS = 2,
    SPINE2 = 3,
    SPINE1 = 6,
    NECK = 80,
    HEAD = 7,
    CLAVICLE_L = 81,
    SHOULDER_L = 9,
    ELBOW_L = 10,
    HAND_L = 11,
    CLAVICLE_R = 82,
    SHOULDER_R = 72,
    ELBOW_R = 14,
    HAND_R = 15,
    HIP_L = 17,
    KNEE_L = 18,
    ANKLE_L = 19,
    HIP_R = 20,
    KNEE_R = 21,
    ANKLE_R = 22,
    BONE_MAX = 128
};

struct bone_link { int a; int b; };

struct BoneIndices_Set {
    int PELVIS;
    int SPINE2;
    int SPINE1;
    int NECK;
    int HEAD;
    int CLAVICLE_L;
    int SHOULDER_L;
    int ELBOW_L;
    int HAND_L;
    int CLAVICLE_R;
    int SHOULDER_R;
    int ELBOW_R;
    int HAND_R;
    int HIP_L;
    int KNEE_L;
    int ANKLE_L;
    int HIP_R;
    int KNEE_R;
    int ANKLE_R;
};

static const BoneIndices_Set BoneIndices_T = {
    2,   3,   6,   80,  7,   81,  9,   10,  11,
    82,  72,  14,  15,  17,  18,  19,  20,  21,  22
};

static const BoneIndices_Set BoneIndices_KT = {
    2,   3,   6,   90,  7,   52,  9,   10,  11,
    13,  74,  14,  15,  17,  18,  19,  20,  21,  22
};

enum : int {
    TEAM_UNASSIGNED = 0,
    TEAM_SPECTATOR  = 1,
    TEAM_T          = 2,
    TEAM_CT         = 3
};

inline const BoneIndices_Set& GetBonesForTeam(int team) {
    return (team == TEAM_T) ? BoneIndices_T : BoneIndices_KT;
}
