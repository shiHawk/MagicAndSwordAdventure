#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 3.5f;
	constexpr float kDashSpeed = 7.0f;
	constexpr float kJumpPower = 7.0f;
	constexpr float kJumpGravity = -0.4f;
	constexpr float kBackLimit = 240.0f;
	constexpr float kFrontLimit = -350.0f;
	constexpr float kLeftLimit = -2660.0f;
	// �����蔻��͈̔�
	constexpr float kColRadius = 40.0f;
	// �ő�HP
	constexpr int kMaxHp = 1000;
	// ����
	constexpr float kMoveDecRate = 0.80f;
	// �J�����ړ��p�̂������l
	constexpr float kMoveCameraThreshold = 60.0f;
	// �A���U���̎�t����
	constexpr float kMaxComboDuration = 120.0f;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kFrontDir = { 0.0,0.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kBackDir = { 0.0,180.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kModelScale = 50.0f; // ���f���̃X�P�[��
	constexpr VECTOR kPlayerInitPos = { -4800.0f, 0.0f, 0.0f }; // �v���C���[�̏����ʒu
	// �A�j���[�V�����̔ԍ�
	constexpr int kIdleAnimNo = 1;
	constexpr int kWalkAnimNo = 3;
	constexpr int kDashAnimNo = 6;
	constexpr int kAttack1AnimNo = 31;
	constexpr int kAttack2AnimNo = 40;
	constexpr int kAttack3AnimNo = 42;
	constexpr int kDamageAnimNo = 24;
	constexpr int kDeathAnimNo = 26;
	constexpr int kJumpAttackAnimNo = 39;
	constexpr int kEvadeAnimNo = 16;
	constexpr int kJumpAnimNo = 10;
	// �A�j���[�V�����̑��x
	constexpr float kAnimSpeedFast = 1.1f; // �Z�߂̍Đ��̎�
	constexpr float kAnimSpeedMedium = 0.5f; // �����x�̃e���|
	constexpr float kAnimSpeedSlow = 1.0f; // ���߂̍Đ��̎�
	// ��������
	constexpr float kAttackDuration = 37.0f;
	constexpr float kEvadeDuration = 40.0f;
	// ��𑬓x�搔
	constexpr float kEvadeSpeedMultiplier = 30.0f;
	constexpr float kEvadeJumpMultiplier = 0.5f;
	// �e�U���̍U����
	constexpr int kFirstAttackPower = 20;
	constexpr int kSecondAttackPower = 40;
	constexpr int kThirdAttackPower = 60;

	constexpr float kWallOffset = 0.001f; // �ǂɓ����������̈ʒu�␳
	constexpr float kAttackHideY = -1000.0f; // ���S���̍U���̈ʒu

	bool isStartGravity = false;
	// �U��Ԃ邾���ňړ������Ɣ��肳���Ȃ����߂�
	bool isMove = false;
	// �A���ňړ��A�j���[�V�������Ă΂Ȃ��悤�ɂ���
	int moveCount = 0;
	int idleCount = 0;
}


Player::Player():
m_damageFrame(0),
m_hp(kMaxHp),
m_rotMtx(MGetIdent()),
m_isJump(false),
m_isDirRight(true),
m_isPrevButton(false),
m_isNowButton(false),
m_jumpCount(0),
m_isAttackDirRight(true),
m_screenPos({0.0f,0.0f,0.0f}),
m_isMovingFlag(false),
m_isNowDirRight(true),
m_isPrevDirRight(true),
m_prevPos({0,0,0}),
m_distanceAfterMoving(0.0f),
attack({ 30,{-500,0,0},false,0.0f,0,30.0f,60.0f,40.0f }),
m_isAttackingAnim(false),
m_isDamageAnim(false),
m_attackDir({0.0f,0.0f,0.0f}),
m_blinkCount(0),
m_isVisible(true)
{
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Animation> pAnimation)
{
	m_pos = kPlayerInitPos;
	m_vec = { 0, 0, 0 };
	m_pAnimation = pAnimation;
	m_modelHandle = MV1LoadModel("Data/model/Barbarian.mv1");
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetRotationXYZ(m_modelHandle, kRightDir);
	m_pAnimation->AttachAnim(m_modelHandle, 1);
	m_prevPos = m_pos;
	m_power = kFirstAttackPower;
	m_hp = kMaxHp;
	m_isDead = false;
	attack = { 35.0f, { -500,0,0 }, false, 0.0f, 0, 30.0f, 60.0f, 40.0f };
}

void Player::End()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Update()
{
	if (m_isDying)
	{
		OnDeath();
		return;
	}
	if (!m_isDead)
	{
		if (m_isDamageAnim)
		{
			// �_���[�W���[�V�������I�������Idle�ɖ߂�
			if (m_pAnimation->GetIsAnimEnd())
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedMedium);
				m_isDamageAnim = false;
			}
			m_pAnimation->UpdateAnim();
			// �_�ŏ���
			if (m_blinkCount > 0)
			{
				m_blinkCount--;

				// 5�t���[�����Ƃɕ\��/��\����؂�ւ���
				if ((m_blinkCount / 5) % 2 == 0)
				{
					m_isVisible = false;
				}
				else
				{
					m_isVisible = true;
				}
			}
			else
			{
				m_isVisible = true; // �_�ŏI����͏�ɕ\��
			}
			return; // �_���[�W���͑��̏������~�߂�(�U����ړ������Ȃ�)
		}
		UpdateCombo();
		if (m_vec.y > 0)
		{
			isStartGravity = true;
		}
		// B�{�^�����������Ƃ�
		m_isNowButton = Pad::isPress(PAD_INPUT_2);
		// B�{�^������������ςȂ��łȂ�
		if (m_isNowButton && !m_isPrevButton && !m_isJump)
		{
			if (isMove) // �ړ����ɍU���������ƈړ������߂�
			{
				isMove = false;
				m_vec.x = 0.0f;
				m_vec.y = 0.0f;
			}
			OnAttack();
		}
		if (attack.active)
		{
			attack.timer--; // �U���̎������Ԃ̌���
			//printfDx("attack.timer:%f\n", attack.timer);
			if (attack.timer <= 0 || m_pAnimation->GetIsAnimEnd())
			{
				attack.pos = { 0.0f,-kAttackHideY,0.0f };
			}
		}
		m_isPrevButton = m_isNowButton; // �{�^���̍X�V
		HandleJump();
		DoMove();
		HandleEvade();
		if (isMove)
		{
			idleCount = 0;
			int targetAnimNo;
			if (Pad::isPress(PAD_INPUT_3))
			{
				targetAnimNo = kDashAnimNo;
			}
			else 
			{
				targetAnimNo = kWalkAnimNo;
			}
			// ���݂̃A�j���ƈႤ�ꍇ�����؂�ւ���
			if (m_pAnimation->GetAttachAnimNo() != targetAnimNo)
			{
				m_pAnimation->ChangeAnim(m_modelHandle, targetAnimNo, true, kAnimSpeedMedium);
			}
		}
		//printfDx("attachAnimNo:%d\n", m_pAnimation->GetAttachAnimNo());

		if (m_pos.y < 0.0f) // �n�ʂɒ��������̕␳
		{
			m_pos.y = 0.0f;
			m_vec.y = 0.0f;
			m_isJump = false;
			m_jumpCount = 0;
		}
		m_pAnimation->UpdateAnim();
		
		// �U�����Ȃ�A�w��J�E���g�ȍ~�ōU������ON
		if (m_isAttackingAnim  &&!attack.active && m_pAnimation->GetPlayTime() >= 4.0f) // �U���A�j���[�V���������܂����肪�o�ĂȂ�
		{
			attack.active = true;
		}
		if (m_isAttackingAnim && m_pAnimation->GetIsAnimEnd()) // �U���A�j���[�V�������Đ����Ă�����Ԃ���A�j���[�V�������I������
		{
			attack.active = false;
			m_isAttackingAnim = false;
			m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedMedium);
		}
	}
	else
	{
		End();
		return;
	}
}

void Player::Draw() const
{
	if (m_isVisible)
	{
		MV1DrawModel(m_modelHandle);
	}
	if (attack.active)
	{
		//DrawSphere3D(attack.pos, attack.radius, 8, 0xff0000, 0xffffff, false);
	}
}

float Player::GetColRadius() const
{
	return kColRadius;
}

void Player::OnDamage(int enemyPower)
{
	if (!evadeData.active)
	{
		m_hp -= enemyPower; // ��𒆂łȂ���΃_���[�W���󂯂�
	}
	if (m_hp <= 0 ) // HP��0�ɂȂ�����
	{
		m_hp = 0;
		m_pAnimation->ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedMedium); // ���S�A�j���[�V�������Đ�
		m_isDying = true;
	}
	else
	{
		m_isAttackingAnim = false;
		attack.active = false;
		m_pAnimation->ChangeAnim(m_modelHandle, kDamageAnimNo, false, kAnimSpeedMedium); // ��e�A�j���[�V�������Đ�
		m_isDamageAnim = true;
		m_blinkCount = 60; // �_�ł��J�n����
	}
	//printfDx(L"hp:%d m_isDying:%d\n", m_hp, m_isDying);
}

void Player::OnDeath()
{
	m_pAnimation->UpdateAnim();
	MV1SetPosition(m_modelHandle, m_pos);
	if (m_pAnimation->GetIsAnimEnd())
	{
		m_isDead = true;
		m_isDying = false;
	}
}

void Player::OnAttack()
{
	if (m_isAttackingAnim && attack.count == 3) return; // 3�i�K�ڂȂ�U�����I���܂Ŏ��̍U�����󂯕t���Ȃ�
	m_isAttackingAnim = true;
	attack.count++;
	// �O��U��������܂����ڂɖ߂�
	if (attack.count > 3)
	{
		attack.count = 1;
	}
	attack.comboDuration = kMaxComboDuration;
	
	switch (attack.count) {
	case 1:
		m_power = kFirstAttackPower; // �U���͂̕ύX
		m_pAnimation->ChangeAnim(m_modelHandle, kAttack1AnimNo, false, kAnimSpeedFast);
		break;
	case 2:
		m_power = kSecondAttackPower;
		m_pAnimation->ChangeAnim(m_modelHandle, kAttack2AnimNo, false, kAnimSpeedFast);
		break;
	case 3:
		m_power = kThirdAttackPower;
		m_pAnimation->ChangeAnim(m_modelHandle, kAttack3AnimNo, false, kAnimSpeedFast + 0.3f);
		break;
	}
	//printfDx(L"attack.count:%d\n", attack.count);
	// �U���ʒu�̌���
	attack.pos.x = m_pos.x + m_attackDir.x * 60.0f;
	attack.pos.y = m_pos.y + attack.attackOffSetY;
	attack.pos.z = m_pos.z + m_attackDir.z * 60.0f;

	attack.timer = kAttackDuration; // �U���������Ԃ̐ݒ�
	// �U������Ƃ��ɏ����O�i����
	if (attack.count == 3)
	{
		m_vec.x += m_attackDir.x * (kMoveSpeed * 1.5f);
		m_vec.z += m_attackDir.z * (kMoveSpeed * 1.5f);
	}
	m_vec.x += m_attackDir.x * (kMoveSpeed * 0.25f);
	m_vec.z += m_attackDir.z * (kMoveSpeed * 0.25f);
}

void Player::OnEvade()
{
	if (evadeData.evadeCount < 1)
	{
		evadeData.active = true;
		// ����A�j���[�V�����ɐ؂�ւ�
		m_pAnimation->ChangeAnim(m_modelHandle, kEvadeAnimNo, false, kAnimSpeedMedium);
		// ����񐔂𑝂₷
		evadeData.evadeCount++;
		evadeData.timer = kEvadeDuration;
		MV1SetRotationXYZ(m_modelHandle, kRightDir); 
		m_vec.x = -kMoveSpeed * kEvadeSpeedMultiplier;
		m_isAttackDirRight = true;
		m_vec.y = kJumpPower * kEvadeJumpMultiplier;
	}
	MV1SetPosition(m_modelHandle, m_pos);
	m_pos = VAdd(m_pos, m_vec);
}

bool Player::IsMoving()
{
	// �U��������狗�������Z�b�g
	if (m_isPrevDirRight != m_isNowDirRight)
	{
		m_prevPos = m_pos;
		m_isMovingFlag = false;
	}
	// �ړ��O�ƈړ���̋����̑傫���𑪂�
	m_distanceAfterMoving = VSize(VSub(m_pos, m_prevPos));
	m_isPrevDirRight = m_isNowDirRight;
	if (!m_isMovingFlag && m_distanceAfterMoving > kMoveCameraThreshold)
	{
		m_isMovingFlag = true;
	}
	return m_isMovingFlag;
}

bool Player::IsPinch()
{
	if (m_hp <= kMaxHp / 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Player::GetMaxHp()
{
	return kMaxHp;
}

void Player::AddPos(VECTOR offset)
{
	m_pos = VAdd(m_pos, offset);
}


void Player::DoMove()
{
	if (isStartGravity)
	{
		m_vec.y += kJumpGravity;
	}
	if (!m_isAttackingAnim)
	{
		HandleInput();
	}
	m_vec.x *= kMoveDecRate;
	m_vec.z *= kMoveDecRate;
	MV1SetPosition(m_modelHandle, m_pos);
	// �ړ���̉����W
	VECTOR nextPos = VAdd(m_pos, m_vec);

	// Z�����̐����@�ړ���̍��W���ǂ��z���Ă�����߂�
	if (nextPos.z >= kBackLimit - kWallOffset)
	{
		nextPos.z = kBackLimit - kWallOffset;
		m_vec.z = 0.0f;
	}
	else if (nextPos.z <= kFrontLimit + kWallOffset)
	{
		nextPos.z = kFrontLimit + kWallOffset;
		m_vec.z = 0.0f;
	}
	// X�����̐����@�ړ���̍��W���ǂ��z���Ă�����߂�
	if (nextPos.x <= kLeftLimit + kWallOffset)
	{
		nextPos.x = kLeftLimit + kWallOffset;
		m_vec.x = 0.0f;
	}

	// �ŏI����
	m_pos = nextPos;
	m_pos = VAdd(m_pos, m_vec);
}

void Player::HandleJump()
{
	if (Pad::isTrigger(PAD_INPUT_1) && !attack.active && !m_isJump)// A�{�^�����������Ƃ��W�����v
	{
		m_vec.y = kJumpPower;
		m_jumpCount++;
		if (!m_isAttackingAnim) // �U�����łȂ���΃W�����v�A�j���[�V�����ɐ؂�ւ�
		{
			m_pAnimation->ChangeAnim(m_modelHandle, kJumpAnimNo, false, kAnimSpeedMedium);
		}
	}
	// �W�����v��2��܂�
	if (m_jumpCount > 1)
	{
		m_isJump = true;
	}
}

void Player::HandleEvade()
{
	//RB�{�^�����������Ƃ�
	if (Pad::isTrigger(PAD_INPUT_6))
	{
		// ���t���[���ړ��ʂ����Z�b�g
		m_vec.x = 0.0f;
		m_vec.z = 0.0f;
		OnEvade();
	}
	if (evadeData.active)
	{
		evadeData.timer--;
		// ������Ԃ��I�������
		if (evadeData.timer <= 0)
		{
			m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedMedium);
			evadeData.active = false;
			evadeData.evadeCount = 0;
		}
	}
}

void Player::UpdateCombo()
{
	if (attack.comboDuration > 0.0f)
	{
		attack.comboDuration--; // �R���{�̎�t���Ԃ����炷
		if (attack.comboDuration <= 0.0f) // �R���{�̎�t���Ԃ��I��������U������i�ڂɖ߂�
		{
			attack.count = 0;
		}
	}
}

void Player::HandleInput()
{
	// ���t���[���ړ��ʂ����Z�b�g
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;
	// ���������̓��͂��`�F�b�N
	if (Pad::isPress(PAD_INPUT_RIGHT) && !evadeData.active)
	{
		m_playerDir = Dir::Right;
		m_isNowDirRight = true;
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.x = kMoveSpeed;
		
		m_isAttackDirRight = true;
		m_isDirRight = true;
	}
	else if (Pad::isPress(PAD_INPUT_LEFT) && !evadeData.active)
	{
		m_playerDir = Dir::Left;
		m_isNowDirRight = false;
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.x = -kMoveSpeed;
		
		m_isAttackDirRight = false;
		m_isDirRight = false;
	}
	// ���������̓��͂��`�F�b�N
	if (Pad::isPress(PAD_INPUT_UP))
	{
		m_playerDir = Dir::Back;
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.z = kMoveSpeed;
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		m_playerDir = Dir::Front;
		attack.pos.z = m_pos.z - 20.0f;
		if (!isMove)
		{
			isMove = true;
		}
		m_vec.z = -kMoveSpeed;
	}
	const bool m_isNowMoving = (m_vec.x != 0.0f || m_vec.z != 0.0f);

	if (m_isNowMoving) // �v���C���[���ړ����Ă���Ȃ�
	{
		attack.count = 0; // �U���̒i�K�����Z�b�g����
		float length = sqrt(m_vec.x * m_vec.x + m_vec.z * m_vec.z);
		if (length > 0.0f)
		{
			m_vec.x /= length; // �ړ��x�N�g���𐳋K��
			m_vec.z /= length;

			m_attackDir.x = m_vec.x; // �U���̕����̍X�V
			m_attackDir.y = 0.0f;
			m_attackDir.z = m_vec.z;
		}
		
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0) // �_�b�V�����͈ړ��x�N�g���𑝉�������
		{
			m_vec.x *= kDashSpeed;
			m_vec.z *= kDashSpeed;
		}
		else
		{
			m_vec.x *= kMoveSpeed;
			m_vec.z *= kMoveSpeed;
		}
		float angle = atan2(m_vec.x, m_vec.z); // �v���C���[�̊p�x���v�Z
		angle += DX_PI_F;
		MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, angle, 0.0f)); // �v���C���[�̕�����ς���
	}
	else
	{
		if (isMove)
		{
			isMove = false;
			moveCount = 0;
			if (idleCount < 1)
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedMedium);
			}
		}
		idleCount++;
	}
}
