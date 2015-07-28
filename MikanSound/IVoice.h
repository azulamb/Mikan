namespace Thaleia
{

	/// PCMプレイヤーのインターフェース
	class IVoice
	{
	public:
		virtual ~IVoice() {}

	public:
		/// PCMをこのインスタンスに結びつける.
		/// 以降, PCMの所有権はこのインスタンスが保持する.
		virtual bool bind( std::unique_ptr<IAudioHandler>&& pcm ) = 0;

		/// PCMとの結びつけを解除する.
		/// PCMの所有権は返却される.
		virtual std::unique_ptr<IAudioHandler> unbind() = 0;

		/// 再生を開始する.
		virtual void play() = 0;

		/// 再生を一時停止する.
		/// 再生位置は保持される.
		virtual void pause() = 0;

		/// 再生を停止する.
		/// 再生位置は先頭に巻き戻る.
		virtual void stop() = 0;

		/// 全体の再生時間をミリ秒で取得する.
		///
		/// returns	全体の再生時間(ミリ秒).
		///			bindされていない場合は0.
		virtual unsigned long size() const = 0;

		/// 現在の再生位置をミリ秒で取得する.
		/// 
		/// returns 現在の再生位置(ミリ秒).
		virtual unsigned long tell() const = 0;

		/// ミリ秒で指定した再生位置に移動する.
		virtual void seek( unsigned long millisecond ) = 0;

		/// 音量を取得する.
		/// 音量は0.0fで無音，1.0fで最大となる.
		virtual double getVolume() const = 0;

		/// パンを取得する
		/// パンは-1.0fで最左, 0.0fで中央, 1.0fで最右となる.
		virtual double getPan() const = 0;

		/// 再生周波数を取得する.
		virtual unsigned int getFrequency() const = 0;

		/// ループの有無を取得する.
		virtual bool isLoopEnabled() const = 0;

		/// ループ位置をミリ秒で取得する.
		virtual bool getLoopTime( unsigned long* start, unsigned long* end ) const = 0;

		/// 音量を設定する.
		/// 音量は0.0fで無音, 1.0fで最大となる.
		virtual void setVolume( double volume ) = 0;

		/// パンを設定する.
		/// パンは-1.0fで最左, 0.0fで中央, 1.0fで最右となる.
		virtual void setPan( double pan ) = 0;

		/// 再生周波数を設定する.
		virtual void setFrequency( unsigned int frequency ) = 0;

		/// ループの有無を設定する.
		/// ループをサポートしない場合はfalseを返し, それ以外ではtrueを返す.
		virtual bool enableLoop( bool enable ) = 0;

		/// ループ位置をミリ秒で設定する.
		/// ループ位置の指定をサポートしない場合falseを返し, それ以外ではtrueを返す.
		virtual bool setLoopTime( unsigned long start, unsigned long end ) = 0;

		/// 現在再生中かどうかを調べる.
		virtual bool isPlaying() const = 0;

	public:
		/// バイト数から時間長を計算する.
		inline static unsigned long toByte(
			unsigned long  millisecond,		/// バイトに変換したい時間長(ミリ秒)
			unsigned short channnels,		/// チャンネル数
			unsigned short bits_per_sample,	/// 1サンプルあたりのビット数
			unsigned long  samples_per_sec	/// 1秒あたりのサンプル数
			)
		{
			// ブロック境界.
			// WAVEFORMATEXの定義より, nBlockAlignはnChannelsとwBitsPerSampleの積を8で割った値.
			unsigned short block_align = channnels * bits_per_sample / 8;

			// 1秒あたりのバイト数.
			unsigned long bytes_per_second = block_align * samples_per_sec;

			// 全体のバイト数.
			unsigned long byte = (unsigned long)( ( ( unsigned long long )bytes_per_second ) * millisecond / 1000 );

			// オーディオ処理はブロック境界を最小単位として行われるので, 境界を跨がないようにする.
			if ( byte % block_align != 0 )
			{
				//LOG("Block-align violation was fixed.");
				byte -= byte % block_align;
			}

			// 完了
			return byte;
		}

		/// 時間長からバイト数を計算する.
		inline static unsigned long toMillisecond(
			unsigned long byte,				/// 時間(ミリ秒)に変換したいバイト数
			unsigned short channnels,		/// チャンネル数
			unsigned short bits_per_sample,	/// 1サンプルあたりのビット数
			unsigned long  samples_per_sec	/// 1秒あたりのサンプル数
			)
		{
			// ブロック境界.
			// WAVEFORMATEXの定義より, nBlockAlignはnChannelsとwBitsPerSampleの積を8で割った値.
			unsigned short block_align = channnels * bits_per_sample / 8;

			// 1秒あたりのバイト数.
			unsigned long bytes_per_second = block_align * samples_per_sec;

			// 全体の時間長(ミリ秒).
			unsigned long millisecond = (unsigned long)( ( ( unsigned long long )byte ) * 1000 / bytes_per_second );
			return millisecond;
		}

	};

};