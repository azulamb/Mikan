namespace Thaleia
{

	/// PCM�v���C���[�̃C���^�[�t�F�[�X
	class IVoice
	{
	public:
		virtual ~IVoice() {}

	public:
		/// PCM�����̃C���X�^���X�Ɍ��т���.
		/// �ȍ~, PCM�̏��L���͂��̃C���X�^���X���ێ�����.
		virtual bool bind( std::unique_ptr<IAudioHandler>&& pcm ) = 0;

		/// PCM�Ƃ̌��т�����������.
		/// PCM�̏��L���͕ԋp�����.
		virtual std::unique_ptr<IAudioHandler> unbind() = 0;

		/// �Đ����J�n����.
		virtual void play() = 0;

		/// �Đ����ꎞ��~����.
		/// �Đ��ʒu�͕ێ������.
		virtual void pause() = 0;

		/// �Đ����~����.
		/// �Đ��ʒu�͐擪�Ɋ����߂�.
		virtual void stop() = 0;

		/// �S�̂̍Đ����Ԃ��~���b�Ŏ擾����.
		///
		/// returns	�S�̂̍Đ�����(�~���b).
		///			bind����Ă��Ȃ��ꍇ��0.
		virtual unsigned long size() const = 0;

		/// ���݂̍Đ��ʒu���~���b�Ŏ擾����.
		/// 
		/// returns ���݂̍Đ��ʒu(�~���b).
		virtual unsigned long tell() const = 0;

		/// �~���b�Ŏw�肵���Đ��ʒu�Ɉړ�����.
		virtual void seek( unsigned long millisecond ) = 0;

		/// ���ʂ��擾����.
		/// ���ʂ�0.0f�Ŗ����C1.0f�ōő�ƂȂ�.
		virtual double getVolume() const = 0;

		/// �p�����擾����
		/// �p����-1.0f�ōō�, 0.0f�Œ���, 1.0f�ōŉE�ƂȂ�.
		virtual double getPan() const = 0;

		/// �Đ����g�����擾����.
		virtual unsigned int getFrequency() const = 0;

		/// ���[�v�̗L�����擾����.
		virtual bool isLoopEnabled() const = 0;

		/// ���[�v�ʒu���~���b�Ŏ擾����.
		virtual bool getLoopTime( unsigned long* start, unsigned long* end ) const = 0;

		/// ���ʂ�ݒ肷��.
		/// ���ʂ�0.0f�Ŗ���, 1.0f�ōő�ƂȂ�.
		virtual void setVolume( double volume ) = 0;

		/// �p����ݒ肷��.
		/// �p����-1.0f�ōō�, 0.0f�Œ���, 1.0f�ōŉE�ƂȂ�.
		virtual void setPan( double pan ) = 0;

		/// �Đ����g����ݒ肷��.
		virtual void setFrequency( unsigned int frequency ) = 0;

		/// ���[�v�̗L����ݒ肷��.
		/// ���[�v���T�|�[�g���Ȃ��ꍇ��false��Ԃ�, ����ȊO�ł�true��Ԃ�.
		virtual bool enableLoop( bool enable ) = 0;

		/// ���[�v�ʒu���~���b�Őݒ肷��.
		/// ���[�v�ʒu�̎w����T�|�[�g���Ȃ��ꍇfalse��Ԃ�, ����ȊO�ł�true��Ԃ�.
		virtual bool setLoopTime( unsigned long start, unsigned long end ) = 0;

		/// ���ݍĐ������ǂ����𒲂ׂ�.
		virtual bool isPlaying() const = 0;

	public:
		/// �o�C�g�����玞�Ԓ����v�Z����.
		inline static unsigned long toByte(
			unsigned long  millisecond,		/// �o�C�g�ɕϊ����������Ԓ�(�~���b)
			unsigned short channnels,		/// �`�����l����
			unsigned short bits_per_sample,	/// 1�T���v��������̃r�b�g��
			unsigned long  samples_per_sec	/// 1�b������̃T���v����
			)
		{
			// �u���b�N���E.
			// WAVEFORMATEX�̒�`���, nBlockAlign��nChannels��wBitsPerSample�̐ς�8�Ŋ������l.
			unsigned short block_align = channnels * bits_per_sample / 8;

			// 1�b������̃o�C�g��.
			unsigned long bytes_per_second = block_align * samples_per_sec;

			// �S�̂̃o�C�g��.
			unsigned long byte = (unsigned long)( ( ( unsigned long long )bytes_per_second ) * millisecond / 1000 );

			// �I�[�f�B�I�����̓u���b�N���E���ŏ��P�ʂƂ��čs����̂�, ���E���ׂ��Ȃ��悤�ɂ���.
			if ( byte % block_align != 0 )
			{
				//LOG("Block-align violation was fixed.");
				byte -= byte % block_align;
			}

			// ����
			return byte;
		}

		/// ���Ԓ�����o�C�g�����v�Z����.
		inline static unsigned long toMillisecond(
			unsigned long byte,				/// ����(�~���b)�ɕϊ��������o�C�g��
			unsigned short channnels,		/// �`�����l����
			unsigned short bits_per_sample,	/// 1�T���v��������̃r�b�g��
			unsigned long  samples_per_sec	/// 1�b������̃T���v����
			)
		{
			// �u���b�N���E.
			// WAVEFORMATEX�̒�`���, nBlockAlign��nChannels��wBitsPerSample�̐ς�8�Ŋ������l.
			unsigned short block_align = channnels * bits_per_sample / 8;

			// 1�b������̃o�C�g��.
			unsigned long bytes_per_second = block_align * samples_per_sec;

			// �S�̂̎��Ԓ�(�~���b).
			unsigned long millisecond = (unsigned long)( ( ( unsigned long long )byte ) * 1000 / bytes_per_second );
			return millisecond;
		}

	};

};