class LightsChannel < ApplicationCable::Channel
  def subscribed
    # stop_all_streams
    stream_from "lights_channel"
  end

  def unsubscribed
  end

  def test(data)
    LightsStatusJob.perform_later(data)
  end
end
