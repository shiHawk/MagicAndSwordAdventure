#include "Player.h"
#include "DxLib.h"
#include "Pad.h"

namespace
{
	constexpr float kMoveSpeed = 4.0f;
	constexpr float kDashSpeed = 7.0f;
	constexpr float kJumpPower = 7.0f;
	constexpr float kJumpGravity = -0.4f;
	constexpr float kBackLimit = 240.0f;
	constexpr float kFrontLimit = -350.0f;
	constexpr float kLeftLimit = -2660.0f;
	// 当たり判定の範囲
	constexpr float kColRadius = 40.0f;
	// 最大HP
	constexpr int kMaxHp = 1000;
	// 減速
	constexpr float kMoveDecRate = 0.80f;
	// カメラ移動用のしきい値
	constexpr float kMoveCameraThreshold = 60.0f;
	// 連続攻撃の受付時間
	constexpr float kMaxComboDuration = 120.0f;

	constexpr VECTOR kRightDir = { 0.0,270.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kLeftDir = { 0.0,90.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kFrontDir = { 0.0,0.0f * DX_PI_F / 180.0f,0.0f };
	constexpr VECTOR kBackDir = { 0.0,180.0f * DX_PI_F / 180.0f,0.0f };
	constexpr float kModelScale = 50.0f; // モデルのスケール
	constexpr VECTOR kPlayerInitPos = { -4800.0f, 0.0f, 0.0f }; // プレイヤーの初期位置
	// アニメーションの番号
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
	// アニメーションの速度
	constexpr float kAnimSpeedFast = 0.9f; // 短めの再生の時
	constexpr float kAnimSpeedMedium = 0.5f; // 中程度のテンポ
	constexpr float kAnimSpeedSlow = 1.0f; // 長めの再生の時
	// 持続時間
	constexpr float kAttackDuration = 37.0f;
	constexpr float kEvadeDuration = 40.0f;
	// 回避速度乗数
	constexpr float kEvadeSpeedMultiplier = 6.0f;
	constexpr float kEvadeJumpMultiplier = 0.5f;
	// 各攻撃の攻撃力
	constexpr int kFirstAttackPower = 20;
	constexpr int kSecondAttackPower = 40;
	constexpr int kThirdAttackPower = 60;

	constexpr float kWallOffset = 0.001f; // 壁に当たった時の位置補正
	constexpr float kAttackHideY = -1000.0f; // 死亡時の攻撃の位置

	bool isStartGravity = false;
	// 振り返るだけで移動したと判定させないために
	bool isMove = false;
	// 連続で移動アニメーションを呼ばないようにする
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
m_isDamageAnim(false)
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
			// ダメージモーションが終わったらIdleに戻す
			if (m_pAnimation->GetIsAnimEnd())
			{
				m_pAnimation->ChangeAnim(m_modelHandle, kIdleAnimNo, true, kAnimSpeedMedium);
				m_isDamageAnim = false;
			}
			m_pAnimation->UpdateAnim();
			return; // ダメージ中は他の処理を止める（攻撃や移動させない）
		}
		UpdateCombo();
		if (m_vec.y > 0)
		{
			isStartGravity = true;
		}
		// Bボタンを押したとき
		m_isNowButton = Pad::isPress(PAD_INPUT_2);
		// Bボタンが押されっぱなしでない
		if (m_isNowButton && !m_isPrevButton && !m_isJump)
		{
			if (isMove) // 移動中に攻撃を押すと移動を辞める
			{
				isMove = false;
				m_vec.x = 0.0f;
				m_vec.y = 0.0f;
			}
			DoAttack();
		}
		if (attack.active)
		{
			attack.timer--;
			//printfDx("attack.timer:%f\n", attack.timer);
			if (attack.timer <= 0 || m_pAnimation->GetIsAnimEnd())
			{
				attack.pos = { 0.0f,-kAttackHideY,0.0f };
			}
		}
		m_isPrevButton = m_isNowButton; // ボタンの更新
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
			// 現在のアニメと違う場合だけ切り替える
			if (m_pAnimation->GetAttachAnimNo() != targetAnimNo)
			{
				m_pAnimation->ChangeAnim(m_modelHandle, targetAnimNo, true, kAnimSpeedMedium);
			}
		}
		//printfDx("attachAnimNo:%d\n", m_pAnimation->GetAttachAnimNo());

		if (m_pos.y < 0.0f) // 地面に着いた時の補正
		{
			m_pos.y = 0.0f;
			m_vec.y = 0.0f;
			m_isJump = false;
			m_jumpCount = 0;
		}
		m_pAnimation->UpdateAnim();
		// 攻撃中なら、指定カウント以降で攻撃判定ON
		if (m_isAttackingAnim  &&!attack.active && m_pAnimation->GetPlayTime() >= 4.0f) // 攻撃アニメーション中かつまだ判定が出てない
		{
			attack.active = true;
		}
		if (m_isAttackingAnim && m_pAnimation->GetIsAnimEnd()) // 攻撃アニメーションを再生していた状態からアニメーションが終了する
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
	MV1DrawModel(m_modelHandle);
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
		m_hp -= enemyPower; // 回避中でなければダメージを受ける
	}
	if (m_hp <= 0 ) // HPが0になったら
	{
		m_hp = 0;
		m_pAnimation->ChangeAnim(m_modelHandle, kDeathAnimNo, false, kAnimSpeedMedium); // 死亡アニメーションを再生
		m_isDying = true;
	}
	else
	{
		m_isAttackingAnim = false;
		attack.active = false;
		m_pAnimation->ChangeAnim(m_modelHandle, kDamageAnimNo, false, kAnimSpeedMedium); // 被弾アニメーションを再生
		m_isDamageAnim = true;
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

void Player::DoAttack()
{
	if (m_isAttackingAnim && attack.count == 3) return; // 攻撃中なら次の攻撃を受け付けない
	m_isAttackingAnim = true;
	attack.count++;
	// 三回攻撃したらまた一回目に戻る
	if (attack.count > 3)
	{
		attack.count = 1;
	}
	attack.comboDuration = kMaxComboDuration;
	if (m_vec.y > 0)
	{
		m_pAnimation->ChangeAnim(m_modelHandle, kJumpAttackAnimNo, true, kAnimSpeedMedium);// ジャンプ攻撃
	}
	else
	{
		switch (attack.count) {
		case 1:
			m_power = kFirstAttackPower; // 攻撃力の変更
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack1AnimNo, false, kAnimSpeedFast);
			break;
		case 2:
			m_power = kSecondAttackPower;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack2AnimNo, false, kAnimSpeedFast+0.2f);
			break;
		case 3:
			m_power = kThirdAttackPower;
			m_pAnimation->ChangeAnim(m_modelHandle, kAttack3AnimNo, false, kAnimSpeedFast+0.3f);
			break;
		}
	}
	//printfDx(L"attack.count:%d\n", attack.count);
	if (m_playerDir == Dir::Right)
	{
		// 攻撃の時に少し前進する
		m_vec.x = +kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x + attack.attackOffSetX;
		attack.pos.z = m_pos.z;
	}
	if(m_playerDir == Dir::Left)
	{
		// 攻撃の時に少し前進する
		m_vec.x = -kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x - attack.attackOffSetX;
		attack.pos.z = m_pos.z;
	}
	if (m_playerDir == Dir::Front)
	{
		// 攻撃の時に少し前進する
		m_vec.z = -kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x;
		attack.pos.z = m_pos.z - 60.0f;
	}
	if (m_playerDir == Dir::Back)
	{
		// 攻撃の時に少し前進する
		m_vec.z = kMoveSpeed * 0.5f;
		attack.pos.x = m_pos.x;
		attack.pos.z = m_pos.z + 60.0f;
	}
	attack.timer = kAttackDuration;
	attack.pos.y = m_pos.y+attack.attackOffSetY;
}

void Player::DoEvade()
{
	if (evadeData.evadeCount < 1)
	{
		evadeData.active = true;
		// 回避アニメーションに切り替え
		m_pAnimation->ChangeAnim(m_modelHandle, kEvadeAnimNo, false, kAnimSpeedMedium);
		// 回避回数を増やす
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
	// 振り向いたら距離をリセット
	if (m_isPrevDirRight != m_isNowDirRight)
	{
		m_prevPos = m_pos;
		m_isMovingFlag = false;
	}
	// 移動前と移動後の距離の大きさを測る
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
	// 移動先の仮座標
	VECTOR nextPos = VAdd(m_pos, m_vec);

	// Z方向の制限　移動先の座標が壁を越えていたら戻す
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
	// X方向の制限　移動先の座標が壁を越えていたら戻す
	if (nextPos.x <= kLeftLimit + kWallOffset)
	{
		nextPos.x = kLeftLimit + kWallOffset;
		m_vec.x = 0.0f;
	}

	// 最終決定
	m_pos = nextPos;
	m_pos = VAdd(m_pos, m_vec);
}

void Player::HandleJump()
{
	if (Pad::isTrigger(PAD_INPUT_1) && !attack.active && !m_isJump)// Aボタンを押したときジャンプ
	{
		m_vec.y = kJumpPower;
		m_jumpCount++;
	}
	// ジャンプは2回まで
	if (m_jumpCount > 1)
	{
		m_isJump = true;
	}
}

void Player::HandleEvade()
{
	//RBボタンを押したとき
	if (Pad::isTrigger(PAD_INPUT_6))
	{
		DoEvade();
	}
	if (evadeData.active)
	{
		evadeData.timer--;
		// 回避時間が終わったら
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
		attack.comboDuration--; // コンボの受付時間を減らす
		if (attack.comboDuration <= 0.0f) // コンボの受付時間が終了したら攻撃を一段目に戻す
		{
			attack.count = 0;
		}
	}
}

void Player::HandleInput()
{
	// 毎フレーム移動量をリセット
	m_vec.x = 0.0f;
	m_vec.z = 0.0f;
	// 垂直方向の入力をチェック
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
	// 水平方向の入力をチェック
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
	const bool isNowMoving = (m_vec.x != 0.0f || m_vec.z != 0.0f);

	if (isNowMoving)
	{
		attack.count = 0;
		float length = sqrt(m_vec.x * m_vec.x + m_vec.z * m_vec.z);
		if (length > 0.0f)
		{
			m_vec.x /= length;
			m_vec.z /= length;
		}
		
		if (Pad::isPress(PAD_INPUT_3) && m_pos.y <= 0)
		{
			m_vec.x *= kDashSpeed;
			m_vec.z *= kDashSpeed;
		}
		else
		{
			m_vec.x *= kMoveSpeed;
			m_vec.z *= kMoveSpeed;
		}
		float angle = atan2(m_vec.x, m_vec.z);
		angle += DX_PI_F;
		MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, angle, 0.0f));
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
