class LightsStatusJob < ApplicationJob
  queue_as :default

  def perform(data)
    @device = Particle.device(ENV['AMBIENT_ID'])
    ActionCable.server.broadcast 'flash_channel', message: "Naprava ni dosegljiva!" unless @device.connected?
    if data[:light] == 'closet' || data[:light] == 'picture'
      state = data[:state] ? 'light-off' : 'light-on'
      device.function(state, data[:light])
    elsif data[:light] == 'ambient'
      state = data[:state] ? 'ambient-off' : 'ambient-on'
      device.function(state, 10000)
    end
    #TODO broadcast new values for checkboxes
  end
end
