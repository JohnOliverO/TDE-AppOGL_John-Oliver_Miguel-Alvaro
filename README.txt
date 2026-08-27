ESTRUTURA

Laboratorio\OpenGL.Local.props
Pessoal\OpenGL.Local.props
OpenGL.props.exemplo

Para usar em uma máquina:
1. Copie a versão correspondente para a pasta da solução.
2. Renomeie para OpenGL.Local.props.
3. Confira os caminhos das bibliotecas.
4. No .vcxproj, use:
   <Import Project="$(ProjectDir)OpenGL.Local.props"
           Condition="Exists('$(ProjectDir)OpenGL.Local.props')" />

Observação:
- OpenGL.Local.props contém caminhos/configurações específicas da máquina.
- OpenGL.props.exemplo serve como modelo para uma nova máquina.
