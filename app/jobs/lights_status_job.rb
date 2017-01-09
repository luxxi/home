class LightsStatusJob < ApplicationJob
  queue_as :default

  def perform(message)
    @device = Particle.device(ENV['AMBIENT_ID'])
    ActionCable.server.broadcast 'flash_channel', message: "Device not connected!" unless @device.connected?
    arr = []
    @device.variables.each_key do |key|
      arr << Hash["id", key, "value", @device.get(key)]
    end
    ActionCable.server.broadcast 'lights_channel', variables: arr
  end
end
